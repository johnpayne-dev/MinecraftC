#pragma once

struct Level;

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
} ParticleData;

typedef struct Entity Particle;

void ParticleCreate(Particle * particle, struct Level * level, float x, float y, float z, float xd, float yd, float zd);
void ParticleTick(Particle * particle);
void ParticleRender(Particle * particle, float t, float x, float y, float z, float v6, float v7);
int ParticleGetParticleTexture(Particle * particle);
