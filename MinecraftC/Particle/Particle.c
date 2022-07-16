#include "Particle.h"
#include "SmokeParticle.h"
#include "TerrainParticle.h"
#include "WaterDropParticle.h"
#include "../Level/Level.h"
#include "../Render/ShapeRenderer.h"

Particle ParticleCreate(Level level, float3 pos, float3 vel) {
	Entity entity = EntityCreate(level);
	entity->type = EntityTypeParticle;
	EntitySetSize(entity, 0.2, 0.2);
	entity->heightOffset = entity->aabbHeight / 2.0;
	EntitySetPosition(entity, pos);
	entity->makeStepSound = false;
	ParticleData this = MemoryAllocate(sizeof(struct ParticleData));
	*this = (struct ParticleData) {
		.color = one3f,
		.delta = vel + (RandomUniform() * 2.0 - 1.0) * 0.4,
		.uv = (float2){ RandomUniform(), RandomUniform() } * 3.0,
		.size = RandomUniform() * 0.5 + 0.5,
		.lifeTime = 4.0 / (RandomUniform() * 0.9 + 0.1),
		.age = 0,
	};
	entity->typeData = this;
	this->delta *= (RandomUniform() + RandomUniform() + 1.0) * 0.15 * 0.4 / length3f(this->delta);
	this->delta.y += 0.1;
	return entity;
}

Particle ParticleSetPower(Particle particle, float power) {
	ParticleData this = particle->typeData;
	this->delta.xz *= power;
	this->delta.y = (this->delta.y - 0.1) * power + 0.1;
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
	
	particle->oldPosition = particle->position;
	if (this->age++ >= this->lifeTime) { EntityRemove(particle); }
	this->delta.y -= 0.04 * this->gravity;
	EntityMove(particle, this->delta);
	this->delta *= 0.98;
	if (particle->onGround) { this->delta.xz *= 0.7; }
}

void ParticleRender(Particle particle, float t, float3 v1, float v6, float v7) {
	ParticleData this = particle->typeData;
	if (this->type == ParticleTypeTerrain) { TerrainParticleRender(particle, t, v1, v6, v7); return; }

	float2 uv0 = { ((this->texture % 16) + this->uv.x / 4.0F) / 16.0F, ((this->texture / 16) + this->uv.y / 4.0F) / 16.0F };
	float2 uv1 = uv0 + 0.0624375;
	float s = 0.1 * this->size;
	float3 v = particle->oldPosition + (particle->position - particle->oldPosition) * t;
	float b = EntityGetBrightness(particle, t);
	ShapeRendererColor(this->color * b);
	ShapeRendererVertexUV(v + v1 * (float3){ -s, -s, -s } - (float3){ v6, 0.0, v7 } * s, (float2){ uv0.x, uv1.y });
	ShapeRendererVertexUV(v + v1 * (float3){ -s, s, -s } + (float3){ v6, 0.0, v7 } * s, (float2){ uv0.x, uv0.y });
	ShapeRendererVertexUV(v + v1 * (float3){ s, s, s } + (float3){ v6, 0.0, v7 } * s, (float2){ uv1.x, uv0.y });
	ShapeRendererVertexUV(v + v1 * (float3){ s, -s, s } - (float3){ v6, 0.0, v7 } * s, (float2){ uv1.x, uv1.y });
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
