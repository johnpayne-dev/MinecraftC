#include "Entity.h"
#include "Level/Level.h"
#include "Player/Player.h"
#include "Utilities/SinTable.h"
#include "Particle/PrimedTNT.h"

Entity EntityCreate(Level level) {
	Entity entity = MemoryAllocate(sizeof(struct Entity));
	*entity = (struct Entity) {
		.onGround = false,
		.horizontalCollision = false,
		.collision = false,
		.slide = true,
		.removed = false,
		.heightOffset = 0.0,
		.aabbWidth = 0.6,
		.aabbHeight = 1.8,
		.oldWalkDistance = 0.0,
		.walkDistance = 0.0,
		.makeStepSound = true,
		.fallDistance = 0.0,
		.nextStep = 1,
		.textureID = 0,
		.ySlideOffset = 0.0,
		.footSize = 0.0,
		.noPhysics = false,
		.pushThrough = 0.0,
		.hovered = false,
		.level = level,
	};
	EntitySetPosition(entity, zero3f);
	return entity;
}

void EntityResetPosition(Entity entity) {
	if (entity->type == EntityTypePlayer) { return PlayerResetPosition(entity); }
	if (entity->level != NULL) {
		float2 spawn = { entity->level->spawn.x + 0.5, entity->level->spawn.y };
		
		for (float z = entity->level->spawn.z + 0.5; z > 0.0; z++) {
			EntitySetPosition(entity, (float3){ spawn.x, spawn.y, z });
			list(AABB) cubes = LevelGetCubes(entity->level, entity->aabb);
			if (ListCount(cubes) == 0) {
				ListDestroy(cubes);
				break;
			}
			ListDestroy(cubes);
		}
		
		entity->delta = zero3f;
		entity->rotation.y = entity->level->spawnRotation;
		entity->rotation.x = 0.0;
	}
}

void EntityRemove(Entity entity) {
	entity->removed = true;
}

void EntitySetSize(Entity entity, float w, float h) {
	entity->aabbWidth = w;
	entity->aabbHeight = h;
}

void EntitySetPosition(Entity entity, float3 pos) {
	entity->position = pos;
	float w = entity->aabbWidth / 2.0;
	float h = entity->aabbHeight / 2.0;
	entity->aabb = (AABB){ .v0 = { pos.x - w, pos.y - h, pos.z - w }, .v1 = { pos.x + w, pos.y + h, pos.z + w } };
}

void EntityTurn(Entity entity, float2 angle) {
	float2 old = entity->rotation;
	entity->rotation += (float2){ -angle.x, angle.y } * 0.15;
	entity->rotation.x = entity->rotation.x < -90.0 ? -90.0 : (entity->rotation.x > 90.0 ? 90.0 : entity->rotation.x);
	entity->oldRotation += entity->rotation - old;
}

void EntityInterpolateTurn(Entity entity, float2 angle) {
	entity->rotation += (float2){ -angle.x, angle.y } * 0.15;
	entity->rotation.x = entity->rotation.x < -90.0 ? -90.0 : (entity->rotation.x > 90.0 ? 90.0 : entity->rotation.x);
}

void EntityTick(Entity entity) {
	if (entity->type == EntityTypeParticle) { ParticleTick(entity); return; }
	if (entity->type == EntityTypePrimedTNT) { PrimedTNTTick(entity); return; }
	
	entity->oldWalkDistance = entity->walkDistance;
	entity->oldPosition = entity->position;
	entity->oldRotation = entity->rotation;
	
	if (entity->type == EntityTypePlayer) { PlayerTick(entity); return; }
}

bool EntityIsFree(Entity entity, float3 a) {
	AABB aabb = AABBMove(entity->aabb, a);
	list(AABB) cubes = LevelGetCubes(entity->level, aabb);
	bool free = ListCount(cubes) > 0 ? false : !LevelContainsAnyLiquid(entity->level, aabb);
	ListDestroy(cubes);
	return free;
}

bool EntityIsFreeScaled(Entity entity, float3 a, float s) {
	AABB aabb = AABBMove(AABBGrow(entity->aabb, one3f * s), a);
	list(AABB) cubes = LevelGetCubes(entity->level, aabb);
	bool free = ListCount(cubes) > 0 ? false : !LevelContainsAnyLiquid(entity->level, aabb);
	ListDestroy(cubes);
	return free;
}

void EntityMove(Entity entity, float3 a) {
	if (entity->noPhysics) {
		entity->aabb = AABBMove(entity->aabb, a);
		entity->position.xz = (entity->aabb.v0.xz + entity->aabb.v1.xz) / 2.0;
		entity->position.y = entity->aabb.v0.y + entity->heightOffset - entity->ySlideOffset;
	} else {
		float2 xz = entity->position.xz;
		float3 old = a;
		AABB oldAABB = entity->aabb;
		list(AABB) cubes = LevelGetCubes(entity->level, AABBExpand(entity->aabb, a));
		
		for (int i = 0; i < ListCount(cubes); i++) {
			AABB aabb = { .v0 = { ((float *)&cubes[i].v0)[0], ((float *)&cubes[i].v0)[1], ((float *)&cubes[i].v0)[2] }, .v1 = { ((float *)&cubes[i].v1)[0], ((float *)&cubes[i].v1)[1], ((float *)&cubes[i].v1)[2] } };
			a.y = AABBClipYCollide(aabb, entity->aabb, a.y);
		}
		entity->aabb = AABBMove(entity->aabb, up3f * a.y);
		if (!entity->slide && old.y != a.y) { a = zero3f; }
		for (int i = 0; i < ListCount(cubes); i++) {
			AABB aabb = { .v0 = { ((float *)&cubes[i].v0)[0], ((float *)&cubes[i].v0)[1], ((float *)&cubes[i].v0)[2] }, .v1 = { ((float *)&cubes[i].v1)[0], ((float *)&cubes[i].v1)[1], ((float *)&cubes[i].v1)[2] } };
			a.x = AABBClipXCollide(aabb, entity->aabb, a.x);
		}
		entity->aabb = AABBMove(entity->aabb, right3f * a.x);
		if (!entity->slide && old.x != a.x) { a = zero3f; }
		for (int i = 0; i < ListCount(cubes); i++) {
			AABB aabb = { .v0 = { ((float *)&cubes[i].v0)[0], ((float *)&cubes[i].v0)[1], ((float *)&cubes[i].v0)[2] }, .v1 = { ((float *)&cubes[i].v1)[0], ((float *)&cubes[i].v1)[1], ((float *)&cubes[i].v1)[2] } };
			a.z = AABBClipZCollide(aabb, entity->aabb, a.z);
		}
		entity->aabb = AABBMove(entity->aabb, forward3f * a.z);
		if (!entity->slide && old.z != a.z) { a = zero3f; }
		bool onGround = entity->onGround || (old.y != a.y && old.y < 0.0);
		ListDestroy(cubes);
		
		if (entity->footSize > 0.0 && onGround && entity->ySlideOffset < 0.05 && (old.x != a.x || old.z != a.z)) {
			float3 b = a;
			a = (float3){ old.x, entity->footSize, old.z };
			AABB tempAABB = entity->aabb;
			entity->aabb = oldAABB;
			list(AABB) cubes = LevelGetCubes(entity->level, AABBExpand(entity->aabb, (float3){ old.x, a.y, old.z }));
			
			for (int i = 0; i < ListCount(cubes); i++) {
				AABB aabb = { .v0 = { ((float *)&cubes[i].v0)[0], ((float *)&cubes[i].v0)[1], ((float *)&cubes[i].v0)[2] }, .v1 = { ((float *)&cubes[i].v1)[0], ((float *)&cubes[i].v1)[1], ((float *)&cubes[i].v1)[2] } };
				a.y = AABBClipYCollide(aabb, entity->aabb, a.y);
			}
			entity->aabb = AABBMove(entity->aabb, up3f * a.y);
			if (!entity->slide && old.y != a.y) { a = zero3f; }
			for (int i = 0; i < ListCount(cubes); i++) {
				AABB aabb = { .v0 = { ((float *)&cubes[i].v0)[0], ((float *)&cubes[i].v0)[1], ((float *)&cubes[i].v0)[2] }, .v1 = { ((float *)&cubes[i].v1)[0], ((float *)&cubes[i].v1)[1], ((float *)&cubes[i].v1)[2] } };
				a.x = AABBClipXCollide(aabb, entity->aabb, a.x);
			}
			entity->aabb = AABBMove(entity->aabb, right3f * a.x);
			if (!entity->slide && old.x != a.x) { a = zero3f; }
			for (int i = 0; i < ListCount(cubes); i++) {
				AABB aabb = { .v0 = { ((float *)&cubes[i].v0)[0], ((float *)&cubes[i].v0)[1], ((float *)&cubes[i].v0)[2] }, .v1 = { ((float *)&cubes[i].v1)[0], ((float *)&cubes[i].v1)[1], ((float *)&cubes[i].v1)[2] } };
				a.z = AABBClipZCollide(aabb, entity->aabb, a.z);
			}
			entity->aabb = AABBMove(entity->aabb, forward3f * a.z);
			if (!entity->slide && old.z != a.z) { a = zero3f; }
			ListDestroy(cubes);
			
			if (b.x * b.x + b.z * b.z >= a.x * a.x + a.z * a.z) {
				a = b;
				entity->aabb = tempAABB;
			} else { entity->ySlideOffset += 0.5; }
		}
		
		entity->horizontalCollision = old.x != a.x || old.z != a.z;
		entity->onGround = old.y != a.y && old.y < 0.0;
		entity->collision = entity->horizontalCollision || old.y != a.y;
		if (entity->onGround) {
			if (entity->fallDistance > 0.0) { entity->fallDistance = 0.0; }
		} else if (a.y < 0.0) { entity->fallDistance -= a.y; }
		
		if (old.x != a.x) { entity->delta.x = 0.0; }
		if (old.y != a.y) { entity->delta.y = 0.0; }
		if (old.z != a.z) { entity->delta.z = 0.0; }
		
		entity->position.xz = (entity->aabb.v0.xz + entity->aabb.v1.xz) / 2.0;
		entity->position.y = entity->aabb.v0.y + entity->heightOffset - entity->ySlideOffset;
		entity->walkDistance += distance2f(entity->position.xz, xz) * 0.6;
		if (entity->makeStepSound) {
			BlockType blockType = LevelGetTile(entity->level, entity->position.x, entity->position.y - entity->heightOffset - 0.2, entity->position.z);
			if (entity->walkDistance > entity->nextStep && blockType > 0) {
				entity->nextStep++;
				TileSound sound = Blocks.table[blockType]->sound;
				if (sound.type != TileSoundTypeNone) {
					EntityPlaySound(entity, "step.", TileSoundGetVolume(sound) * 0.75, TileSoundGetPitch(sound));
				}
			}
		}
		entity->ySlideOffset *= 0.4;
	}
}

bool EntityIsInWater(Entity entity) {
	return LevelContainsLiquid(entity->level, AABBGrow(entity->aabb, up3f * -0.4), LiquidTypeWater);
}

bool EntityIsUnderWater(Entity entity) {
	BlockType blockType = LevelGetTile(entity->level, entity->position.x, entity->position.y, entity->position.z);
	return blockType != 0 ? BlockGetLiquidType(Blocks.table[blockType]) == LiquidTypeWater : false;
}

bool EntityIsInLava(Entity entity) {
	return LevelContainsLiquid(entity->level, AABBGrow(entity->aabb, up3f * -0.4), LiquidTypeLava);
}

void EntityMoveRelative(Entity entity, float2 xz, float speed) {
	float len = length2f(xz);
	if (len >= 0.01) {
		if (len < 1.0) { len = 1.0; }
		len = speed / len;
		xz *= len;
		float s = tsin(entity->rotation.y * rad);
		float c = tcos(entity->rotation.y * rad);
		entity->delta.x += xz.x * c - xz.y * s;
		entity->delta.z += xz.y * c + xz.x * s;
	}
}

bool EntityIsLit(Entity entity) {
	return LevelIsLit(entity->level, entity->position.x, entity->position.y, entity->position.z);
}

float EntityGetBrightness(Entity entity, float t) {
	return LevelGetBrightness(entity->level, entity->position.x, entity->position.y, entity->position.z);
}

void EntitySetLevel(Entity entity, Level level) {
	entity->level = level;
}

void EntityPlaySound(Entity entity, const char * name, float volume, float pitch) {
	LevelPlaySound(entity->level, name, entity, volume, pitch);
}

void EntityMoveTo(Entity entity, float3 pos, float2 rot) {
	entity->oldPosition = entity->position;
	entity->position = pos;
	entity->rotation = rot;
	EntitySetPosition(entity, pos);
}

float EntityDistanceTo(Entity entityA, Entity entityB) {
	return distance3f(entityA->position, entityB->position);
}

float EntityDistanceToPoint(Entity entity, float3 point) {
	return distance3f(entity->position, point);
}

float EntitySquaredDistanceTo(Entity entityA, Entity entityB) {
	return sqdistance3f(entityA->position, entityB->position);
}

void EntityPushTowards(Entity entity, float3 point) {
	entity->delta += point;
}

bool EntityIntersects(Entity entity, float3 v0, float3 v1) {
	return AABBIntersects(entity->aabb, (AABB){ v0, v1 });
}

bool EntityShouldRender(Entity entity, float3 v) {
	return EntityShouldRenderAtSquaredDistance(entity, sqdistance3f(entity->position, v));
}

bool EntityShouldRenderAtSquaredDistance(Entity entity, float v) {
	return v < pow(AABBGetSize(entity->aabb) * 64.0, 2.0);
}

bool EntityIsPickable(Entity entity) {
	if (entity->type == EntityTypePrimedTNT) { return PrimedTNTIsPickable(entity); }
	return false;
}

void EntityRender(Entity entity, TextureManager textures, float t) {
	if (entity->type == EntityTypePrimedTNT) { PrimedTNTRender(entity, textures, t); return; }
}

int EntityGetTexture(Entity entity) {
	return entity->textureID;
}

void EntityDestroy(Entity entity) {
	if (entity->type == EntityTypePlayer) { PlayerDestroy(entity); }
	if (entity->type == EntityTypeParticle) { ParticleDestroy(entity); }
	MemoryFree(entity);
}
