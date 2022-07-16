#include "PrimedTNT.h"
#include "SmokeParticle.h"
#include "TerrainParticle.h"
#include "../Utilities/SinTable.h"
#include "../Utilities/OpenGL.h"

PrimedTNT PrimedTNTCreate(Level level, float3 pos) {
	Entity entity = EntityCreate(level);
	entity->type = EntityTypePrimedTNT;
	entity->typeData = MemoryAllocate(sizeof(struct PrimedTNTData));
	EntitySetSize(entity, 0.98, 0.98);
	entity->heightOffset = entity->aabbHeight / 2.0;
	EntitySetPosition(entity, pos);
	entity->makeStepSound = false;
	entity->oldPosition = pos;
	float r = RandomUniform() * 2.0 * pi;
	PrimedTNTData this = entity->typeData;
	this->delta = (float3){ -tsin(r * pi / 180.0) * 0.02, 0.2, -cos(r * pi / 180.0) * 0.02 };
	this->life = 40;
	this->defused = false;
	return entity;
}

void PrimedTNTHurt(PrimedTNT entity) {
	if (!entity->removed) { EntityRemove(entity); }
}

bool PrimedTNTIsPickable(PrimedTNT entity) {
	return !entity->removed;
}

void PrimedTNTTick(PrimedTNT entity) {
	PrimedTNTData this = entity->typeData;
	entity->oldPosition = entity->position;
	this->delta.y -= 0.04;
	EntityMove(entity, this->delta);
	this->delta *= 0.98;
	if (entity->onGround) { this->delta *= (float3){ 0.7, -0.5, 0.7 }; }
	
	if (!this->defused) {
		this->life--;
		if (this->life > 0) {
			ParticleManagerSpawnParticle(entity->level->particleEngine, SmokeParticleCreate(entity->level, entity->position + up3f * 0.6));
		} else {
			EntityRemove(entity);
			RandomGenerator random = RandomGeneratorCreate(time(NULL));
			float radius = 4.0;
			LevelExplode(entity->level, NULL, entity->position, radius);
			for (int i = 0; i < 100; i++) {
				float3 offset = (float3){ RandomGeneratorNormal(random, 1.0), RandomGeneratorNormal(random, 1.0), RandomGeneratorNormal(random, 1.0) } * radius / 4.0;
				float3 vel = offset / dot3f(offset, offset);
				ParticleManagerSpawnParticle(entity->level->particleEngine, TerrainParticleCreate(entity->level, entity->position + offset, vel, Blocks.table[BlockTypeTNT]));
			}
			RandomGeneratorDestroy(random);
		}
	}
}

void PrimedTNTRender(PrimedTNT tnt, TextureManager textures, float t) {
	PrimedTNTData this = tnt->typeData;
	int texture = TextureManagerLoad(textures, "Terrain.png");
	glBindTexture(GL_TEXTURE_2D, texture);
	float brightness = LevelGetBrightness(tnt->level, tnt->position.x, tnt->position.y, tnt->position.z);
	glPushMatrix();
	glColor4f(brightness, brightness, brightness, 1.0);
	float3 v = tnt->oldPosition + (tnt->position - tnt->oldPosition) * t - 0.5;
	glTranslatef(v.x, v.y, v.z);
	glPushMatrix();
	BlockRenderPreview(Blocks.table[BlockTypeTNT]);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glColor4f(1.0, 1.0, 1.0, ((this->life / 4 + 1) % 2) * 0.4);
	if (this->life <= 16) { glColor4f(1.0, 1.0, 1.0, ((this->life + 1) % 2) * 0.6); }
	if (this->life <= 2) { glColor4f(1.0, 1.0, 1.0, 0.9); }
		
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	BlockRenderPreview(Blocks.table[BlockTypeTNT]);
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPopMatrix();
}

void PrimedTNTDestroy(PrimedTNT tnt) {
	MemoryFree(tnt->typeData);
}
