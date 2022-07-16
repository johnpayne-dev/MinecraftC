#pragma once
#include "../Entity.h"

typedef enum ParticleType {
	ParticleTypeNone,
	ParticleTypeSmoke,
	ParticleTypeTerrain,
	ParticleTypeWaterDrop,
} ParticleType;

typedef struct ParticleData {
	float3 delta;
	int texture;
	float2 uv;
	int age;
	int lifeTime;
	float size;
	float gravity;
	float3 color;
	ParticleType type;
} * ParticleData;

typedef Entity Particle;

Particle ParticleCreate(struct Level * level, float3 pos, float3 vel);
Particle ParticleSetPower(Particle particle, float power);
Particle ParticleScale(Particle particle, float scale);
void ParticleTick(Particle particle);
void ParticleRender(Particle particle, float t, float3 v1, float v6, float v7);
int ParticleGetParticleTexture(Particle particle);
void ParticleDestroy(Particle particle);
