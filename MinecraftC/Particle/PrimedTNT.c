#include "PrimedTNT.h"
#include "SmokeParticle.h"
#include "TerrainParticle.h"
#include "../Utilities/SinTable.h"
#include "../Utilities/OpenGL.h"

PrimedTNT PrimedTNTCreate(Level level, float x, float y, float z) {
	Entity entity = EntityCreate(level);
	entity->type = EntityTypePrimedTNT;
	entity->typeData = MemoryAllocate(sizeof(struct PrimedTNTData));
	EntitySetSize(entity, 0.98, 0.98);
	entity->heightOffset = entity->aabbHeight / 2.0;
	EntitySetPosition(entity, x, y, z);
	entity->makeStepSound = false;
	entity->xo = x;
	entity->yo = y;
	entity->zo = z;
	float r = RandomUniform() * 2.0 * M_PI;
	PrimedTNTData this = entity->typeData;
	this->xd = -tsin(r * M_PI / 180.0) * 0.02;
	this->yd = 0.2;
	this->zd = -cos(r * M_PI / 180.0) * 0.02;
	this->life = 40;
	this->defused = false;
	return entity;
}

void PrimedTNTOnHit(PrimedTNT entity) {
	if (!entity->removed) { EntityRemove(entity); }
}

bool PrimedTNTIsPickable(PrimedTNT entity) {
	return !entity->removed;
}

void PrimedTNTTick(PrimedTNT entity) {
	PrimedTNTData this = entity->typeData;
	entity->xo = entity->x;
	entity->yo = entity->y;
	entity->zo = entity->z;
	this->yd -= 0.04;
	EntityMove(entity, this->xd, this->yd, this->zd);
	this->xd *= 0.98;
	this->yd *= 0.98;
	this->zd *= 0.98;
	if (entity->onGround) {
		this->xd *= 0.7;
		this->yd *= -0.5;
		this->zd *= 0.7;
	}
	
	if (!this->defused) {
		this->life--;
		if (this->life > 0) {
			ParticleManagerSpawnParticle(entity->level->particleEngine, SmokeParticleCreate(entity->level, entity->x, entity->y + 0.6, entity->z));
		} else {
			EntityRemove(entity);
			RandomGenerator random = RandomGeneratorCreate(time(NULL));
			float radius = 4.0;
			LevelExplode(entity->level, NULL, entity->x, entity->y, entity->z, radius);
			for (int i = 0; i < 100; i++) {
				float ox = RandomGeneratorNormal(random, 1.0) * radius / 4.0;
				float oy = RandomGeneratorNormal(random, 1.0) * radius / 4.0;
				float oz = RandomGeneratorNormal(random, 1.0) * radius / 4.0;
				float l = ox * ox + oy * oy + oz * oz;
				ParticleManagerSpawnParticle(entity->level->particleEngine, TerrainParticleCreate(entity->level, entity->x + ox, entity->y + oy, entity->z + oz, ox / l, oy / l, oz / l, Blocks.table[BlockTypeTNT]));
			}
			RandomGeneratorDestroy(random);
		}
	}
}

void PrimedTNTRender(PrimedTNT tnt, TextureManager textures, float dt) {
	PrimedTNTData this = tnt->typeData;
	int texture = TextureManagerLoad(textures, "Terrain.png");
	glBindTexture(GL_TEXTURE_2D, texture);
	float brightness = LevelGetBrightness(tnt->level, tnt->x, tnt->y, tnt->z);
	glPushMatrix();
	glColor4f(brightness, brightness, brightness, 1.0);
	float vx = tnt->xo + (tnt->x - tnt->xo) * dt - 0.5;
	float vy = tnt->yo + (tnt->y - tnt->yo) * dt - 0.5;
	float vz = tnt->zo + (tnt->z - tnt->zo) * dt - 0.5;
	glTranslatef(vx, vy, vz);
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
