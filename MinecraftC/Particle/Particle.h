#pragma once
#include "../Entity.h"

typedef enum ParticleType {
	ParticleTypeNone,
	ParticleTypeSmoke,
	ParticleTypeTerrain,
	ParticleTypeWaterDrop,
} ParticleType;

typedef struct ParticleData {
	float xd, yd, zd;
	int texture;
	float u, v;
	int age;
	int lifeTime;
	float size;
	float gravity;
	float r, g, b;
	ParticleType type;
} * ParticleData;

typedef Entity Particle;

Particle ParticleCreate(struct Level * level, float x, float y, float z, float xd, float yd, float zd);
Particle ParticleSetPower(Particle particle, float power);
Particle ParticleScale(Particle particle, float scale);
void ParticleTick(Particle particle);
void ParticleRender(Particle particle, float t, float x, float y, float z, float v6, float v7);
int ParticleGetParticleTexture(Particle particle);
void ParticleDestroy(Particle particle);
