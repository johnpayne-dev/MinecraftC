#pragma once
#include "../Entity.h"

typedef enum ParticleType
{
	ParticleTypeNone,
	ParticleTypeSmoke,
	ParticleTypeTerrain,
	ParticleTypeWaterDrop,
} ParticleType;

typedef struct ParticleData
{
	float3 Delta;
	int Texture;
	float2 UV;
	int Age;
	int LifeTime;
	float Size;
	float Gravity;
	float3 Color;
	ParticleType Type;
} * ParticleData;

typedef Entity Particle;

Particle ParticleCreate(struct Level * level, float3 pos, float3 vel);
Particle ParticleSetPower(Particle particle, float power);
Particle ParticleScale(Particle particle, float scale);
void ParticleTick(Particle particle);
void ParticleRender(Particle particle, float t, float3 v1, float v6, float v7);
int ParticleGetParticleTexture(Particle particle);
void ParticleDestroy(Particle particle);
