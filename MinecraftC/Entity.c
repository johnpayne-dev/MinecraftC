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
	EntitySetPosition(entity, 0.0, 0.0, 0.0);
	return entity;
}

void EntityResetPosition(Entity entity) {
	if (entity->type == EntityTypePlayer) { return PlayerResetPosition(entity); }
	if (entity->level != NULL) {
		float spawnX = entity->level->xSpawn + 0.5;
		float spawnY = entity->level->ySpawn;
		
		for (float z = entity->level->zSpawn + 0.5; z > 0.0; z++) {
			EntitySetPosition(entity, spawnX, spawnY, z);
			list(AABB) cubes = LevelGetCubes(entity->level, entity->aabb);
			if (ListCount(cubes) == 0) {
				ListDestroy(cubes);
				break;
			}
			ListDestroy(cubes);
		}
		
		entity->xd = 0.0;
		entity->yd = 0.0;
		entity->zd = 0.0;
		entity->yRot = entity->level->spawnRotation;
		entity->xRot = 0.0;
	}
}

void EntityRemove(Entity entity) {
	entity->removed = true;
}

void EntitySetSize(Entity entity, float w, float h) {
	entity->aabbWidth = w;
	entity->aabbHeight = h;
}

void EntitySetPosition(Entity entity, float x, float y, float z) {
	entity->x = x;
	entity->y = y;
	entity->z = z;
	float w = entity->aabbWidth / 2.0;
	float h = entity->aabbHeight / 2.0;
	entity->aabb = (AABB){ .x0 = x - w, .y0 = y - h, .z0 = z - w, .x1 = x + w, .y1 = y + h, .z1 = z + w };
}

void EntityTurn(Entity entity, float rx, float ry) {
	float orx = entity->xRot;
	float ory = entity->yRot;
	entity->xRot -= rx * 0.15;
	entity->yRot += ry * 0.15;
	entity->xRot = entity->xRot < -90.0 ? -90.0 : (entity->xRot > 90.0 ? 90.0 : entity->xRot);
	entity->xRotO += entity->xRot - orx;
	entity->yRotO += entity->yRot - ory;
}

void EntityInterpolateTurn(Entity entity, float rx, float ry) {
	entity->xRot -= rx * 0.15;
	entity->yRot += ry * 0.15;
	entity->xRot = entity->xRot < -90.0 ? -90.0 : (entity->xRot > 90.0 ? 90.0 : entity->xRot);
}

void EntityTick(Entity entity) {
	if (entity->type == EntityTypeParticle) { ParticleTick(entity); return; }
	if (entity->type == EntityTypePrimedTNT) { PrimedTNTTick(entity); return; }
	
	entity->oldWalkDistance = entity->walkDistance;
	entity->xo = entity->x;
	entity->yo = entity->y;
	entity->zo = entity->z;
	entity->xRotO = entity->xRot;
	entity->yRotO = entity->yRot;
	
	if (entity->type == EntityTypePlayer) { PlayerTick(entity); return; }
}

bool EntityIsFree(Entity entity, float ax, float ay, float az) {
	AABB aabb = AABBMove(entity->aabb, ax, ay, az);
	list(AABB) cubes = LevelGetCubes(entity->level, aabb);
	bool free = ListCount(cubes) > 0 ? false : !LevelContainsAnyLiquid(entity->level, aabb);
	ListDestroy(cubes);
	return free;
}

bool EntityIsFreeScaled(Entity entity, float ax, float ay, float az, float s) {
	AABB aabb = AABBMove(AABBGrow(entity->aabb, s, s, s), ax, ay, az);
	list(AABB) cubes = LevelGetCubes(entity->level, aabb);
	bool free = ListCount(cubes) > 0 ? false : !LevelContainsAnyLiquid(entity->level, aabb);
	ListDestroy(cubes);
	return free;
}

void EntityMove(Entity entity, float ax, float ay, float az) {
	if (entity->noPhysics) {
		entity->aabb = AABBMove(entity->aabb, ax, ay, az);
		entity->x = (entity->aabb.x0 + entity->aabb.x1) / 2.0;
		entity->z = (entity->aabb.z0 + entity->aabb.z1) / 2.0;
		entity->y = entity->aabb.y0 + entity->heightOffset - entity->ySlideOffset;
	} else {
		float x = entity->x;
		float z = entity->z;
		float ox = ax;
		float oy = ay;
		float oz = az;
		AABB oldAABB = entity->aabb;
		list(AABB) cubes = LevelGetCubes(entity->level, AABBExpand(entity->aabb, ax, ay, az));
		
		for (int i = 0; i < ListCount(cubes); i++) {
			AABB aabb = { .x0 = cubes[i].x0, .y0 = cubes[i].y0, .z0 = cubes[i].z0, .x1 = cubes[i].x1, .y1 = cubes[i].y1, .z1 = cubes[i].z1 };
			ay = AABBClipYCollide(aabb, entity->aabb, ay);
		}
		entity->aabb = AABBMove(entity->aabb, 0.0, ay, 0.0);
		if (!entity->slide && oy != ay) {
			ax = 0.0;
			ay = 0.0;
			az = 0.0;
		}
		for (int i = 0; i < ListCount(cubes); i++) {
			AABB aabb = { .x0 = cubes[i].x0, .y0 = cubes[i].y0, .z0 = cubes[i].z0, .x1 = cubes[i].x1, .y1 = cubes[i].y1, .z1 = cubes[i].z1 };
			ax = AABBClipXCollide(aabb, entity->aabb, ax);
		}
		entity->aabb = AABBMove(entity->aabb, ax, 0.0, 0.0);
		if (!entity->slide && ox != ax) {
			ax = 0.0;
			ay = 0.0;
			az = 0.0;
		}
		for (int i = 0; i < ListCount(cubes); i++) {
			AABB aabb = { .x0 = cubes[i].x0, .y0 = cubes[i].y0, .z0 = cubes[i].z0, .x1 = cubes[i].x1, .y1 = cubes[i].y1, .z1 = cubes[i].z1 };
			az = AABBClipZCollide(aabb, entity->aabb, az);
		}
		entity->aabb = AABBMove(entity->aabb, 0.0, 0.0, az);
		if (!entity->slide && oz != az) {
			ax = 0.0;
			ay = 0.0;
			az = 0.0;
		}
		bool onGround = entity->onGround || (oy != ay && oy < 0.0);
		ListDestroy(cubes);
		
		if (entity->footSize > 0.0 && onGround && entity->ySlideOffset < 0.05 && (ox != ax || oz != az)) {
			float bx = ax;
			float by = ay;
			float bz = az;
			ax = ox;
			ay = entity->footSize;
			az = oz;
			AABB tempAABB = entity->aabb;
			entity->aabb = oldAABB;
			list(AABB) cubes = LevelGetCubes(entity->level, AABBExpand(entity->aabb, ox, ay, oz));
			
			for (int i = 0; i < ListCount(cubes); i++) {
				AABB aabb = { .x0 = cubes[i].x0, .y0 = cubes[i].y0, .z0 = cubes[i].z0, .x1 = cubes[i].x1, .y1 = cubes[i].y1, .z1 = cubes[i].z1 };
				ay = AABBClipYCollide(aabb, entity->aabb, ay);
			}
			entity->aabb = AABBMove(entity->aabb, 0.0, ay, 0.0);
			if (!entity->slide && oy != ay) {
				ax = 0.0;
				ay = 0.0;
				az = 0.0;
			}
			for (int i = 0; i < ListCount(cubes); i++) {
				AABB aabb = { .x0 = cubes[i].x0, .y0 = cubes[i].y0, .z0 = cubes[i].z0, .x1 = cubes[i].x1, .y1 = cubes[i].y1, .z1 = cubes[i].z1 };
				ax = AABBClipXCollide(aabb, entity->aabb, ax);
			}
			entity->aabb = AABBMove(entity->aabb, ax, 0.0, 0.0);
			if (!entity->slide && ox != ax) {
				ax = 0.0;
				ay = 0.0;
				az = 0.0;
			}
			for (int i = 0; i < ListCount(cubes); i++) {
				AABB aabb = { .x0 = cubes[i].x0, .y0 = cubes[i].y0, .z0 = cubes[i].z0, .x1 = cubes[i].x1, .y1 = cubes[i].y1, .z1 = cubes[i].z1 };
				az = AABBClipZCollide(aabb, entity->aabb, az);
			}
			entity->aabb = AABBMove(entity->aabb, 0.0, 0.0, az);
			if (!entity->slide && oz != az) {
				ax = 0.0;
				ay = 0.0;
				az = 0.0;
			}
			ListDestroy(cubes);
			
			if (bx * bx + bz * bz >= ax * ax + az * az) {
				ax = bx;
				ay = by;
				az = bz;
				entity->aabb = tempAABB;
			} else { entity->ySlideOffset += 0.5; }
		}
		
		entity->horizontalCollision = ox != ax || oz != az;
		entity->onGround = oy != ay && oy < 0.0;
		entity->collision = entity->horizontalCollision || oy != ay;
		if (entity->onGround) {
			if (entity->fallDistance > 0.0) { entity->fallDistance = 0.0; }
		} else if (ay < 0.0) { entity->fallDistance -= ay; }
		
		if (ox != ax) { entity->xd = 0.0; }
		if (oy != ay) { entity->yd = 0.0; }
		if (oz != az) { entity->zd = 0.0; }
		
		entity->x = (entity->aabb.x0 + entity->aabb.x1) / 2.0;
		entity->z = (entity->aabb.z0 + entity->aabb.z1) / 2.0;
		entity->y = entity->aabb.y0 + entity->heightOffset - entity->ySlideOffset;
		entity->walkDistance += sqrtf((entity->x - x) * (entity->x - x) + (entity->z - z) * (entity->z - z)) * 0.6;
		if (entity->makeStepSound) {
			BlockType blockType = LevelGetTile(entity->level, entity->x, entity->y - entity->heightOffset - 0.2, entity->z);
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
	return LevelContainsLiquid(entity->level, AABBGrow(entity->aabb, 0.0, -0.4, 0.0), LiquidTypeWater);
}

bool EntityIsUnderWater(Entity entity) {
	BlockType blockType = LevelGetTile(entity->level, entity->x, entity->y, entity->z);
	return blockType != 0 ? BlockGetLiquidType(Blocks.table[blockType]) == LiquidTypeWater : false;
}

bool EntityIsInLava(Entity entity) {
	return LevelContainsLiquid(entity->level, AABBGrow(entity->aabb, 0.0, -0.4, 0.0), LiquidTypeLava);
}

void EntityMoveRelative(Entity entity, float x, float z, float speed) {
	float len = sqrtf(x * x + z * z);
	if (len >= 0.01) {
		if (len < 1.0) { len = 1.0; }
		len = speed / len;
		x *= len;
		z *= len;
		float s = tsin(entity->yRot * M_PI / 180.0);
		float c = tcos(entity->yRot * M_PI / 180.0);
		entity->xd += x * c - z * s;
		entity->zd += z * c + x * s;
	}
}

bool EntityIsLit(Entity entity) {
	return LevelIsLit(entity->level, entity->x, entity->y, entity->z);
}

float EntityGetBrightness(Entity entity, float t) {
	return LevelGetBrightness(entity->level, entity->x, entity->y, entity->z);
}

void EntitySetLevel(Entity entity, Level level) {
	entity->level = level;
}

void EntityPlaySound(Entity entity, const char * name, float volume, float pitch) {
	LevelPlaySound(entity->level, name, entity, volume, pitch);
}

void EntityMoveTo(Entity entity, float x, float y, float z, float rx, float ry) {
	entity->xo = entity->x;
	entity->yo = entity->y;
	entity->zo = entity->z;
	entity->x = x;
	entity->y = y;
	entity->z = z;
	entity->xRot = rx;
	entity->yRot = ry;
	EntitySetPosition(entity, x, y, z);
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
