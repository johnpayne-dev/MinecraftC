#include "Particle.h"
#include "SmokeParticle.h"
#include "TerrainParticle.h"
#include "WaterDropParticle.h"
#include "../Level/Level.h"
#include "../Render/ShapeRenderer.h"

Particle ParticleCreate(Level level, float x, float y, float z, float xd, float yd, float zd) {
	Entity entity = EntityCreate(level);
	entity->type = EntityTypeParticle;
	EntitySetSize(entity, 0.2, 0.2);
	entity->heightOffset = entity->aabbHeight / 2.0;
	EntitySetPosition(entity, x, y, z);
	entity->makeStepSound = false;
	ParticleData this = MemoryAllocate(sizeof(struct ParticleData));
	*this = (struct ParticleData) {
		.r = 1.0,
		.g = 1.0,
		.b = 1.0,
		.xd = xd + (RandomUniform() * 2.0 - 1.0) * 0.4,
		.yd = yd + (RandomUniform() * 2.0 - 1.0) * 0.4,
		.zd = zd + (RandomUniform() * 2.0 - 1.0) * 0.4,
		.u = RandomUniform() * 3.0,
		.v = RandomUniform() * 3.0,
		.size = RandomUniform() * 0.5 + 0.5,
		.lifeTime = 4.0 / (RandomUniform() * 0.9 + 0.1),
		.age = 0,
	};
	entity->typeData = this;
	float s = (RandomUniform() + RandomUniform() + 1.0) * 0.15 * 0.4 / sqrtf(this->xd * this->xd + this->yd * this->yd + this->zd * this->zd);
	this->xd *= s;
	this->yd *= s;
	this->zd *= s;
	this->yd += 0.1;
	return entity;
}

Particle ParticleSetPower(Particle particle, float power) {
	ParticleData this = particle->typeData;
	this->xd *= power;
	this->zd *= power;
	this->yd = (this->yd - 0.1) * power + 0.1;
	return particle;
}

Particle ParticleScale(Particle particle, float scale) {
	ParticleData this = particle->typeData;
	EntitySetSize(particle, 0.2 * scale, 0.2 * scale);
	this->size *= scale;
	return particle;
}

void ParticleTick(Particle particle) {
	ParticleData this = particle->typeData;
	if (this->type == ParticleTypeSmoke) { SmokeParticleTick(particle); }
	if (this->type == ParticleTypeWaterDrop) { WaterDropParticleTick(particle); }
	
	particle->xo = particle->x;
	particle->yo = particle->y;
	particle->zo = particle->z;
	if (this->age++ >= this->lifeTime) { EntityRemove(particle); }
	this->yd -= 0.04 * this->gravity;
	EntityMove(particle, this->xd, this->yd, this->zd);
	this->xd *= 0.98;
	this->yd *= 0.98;
	this->zd *= 0.98;
	if (particle->onGround) {
		this->xd *= 0.7;
		this->zd *= 0.7;
	}
}

void ParticleRender(Particle particle, float dt, float x, float y, float z, float v6, float v7) {
	ParticleData this = particle->typeData;
	if (this->type == ParticleTypeTerrain) { TerrainParticleRender(particle, dt, x, y, z, v6, v7); return; }

	float u0 = ((this->texture % 16) + this->u / 4.0) / 16.0;
	float v0 = ((this->texture / 16) + this->v / 4.0) / 16.0;
	float u1 = u0 + 0.0624375;
	float v1 = v0 + 0.0624375;
	float s = 0.1 * this->size;
	float vx = particle->xo + (particle->x - particle->xo) * dt;
	float vy = particle->yo + (particle->y - particle->yo) * dt;
	float vz = particle->zo + (particle->z - particle->zo) * dt;
	float brightness = EntityGetBrightness(particle, dt);
	ShapeRendererColorf(this->r * brightness, this->g * brightness, this->b * brightness);
	ShapeRendererVertexUV(vx - x * s - v6 * s, vy - y * s, vz - z * s - v7 * s, u0, v1);
	ShapeRendererVertexUV(vx - x * s + v6 * s, vy + y * s, vz - z * s + v7 * s, u0, v0);
	ShapeRendererVertexUV(vx + x * s + v6 * s, vy + y * s, vz + z * s + v7 * s, u1, v0);
	ShapeRendererVertexUV(vx + x * s - v6 * s, vy - y * s, vz + z * s - v7 * s, u1, v1);
}

int ParticleGetParticleTexture(Particle particle) {
	ParticleData this = particle->typeData;
	if (this->type == ParticleTypeTerrain) { return TerrainParticleGetTexture(particle); }
	return 0;
}

void ParticleDestroy(Particle particle) {
	ParticleData this = particle->typeData;
	MemoryFree(this);
}
