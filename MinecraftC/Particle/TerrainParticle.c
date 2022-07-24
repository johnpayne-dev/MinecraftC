#include "TerrainParticle.h"
#include "../Level/Level.h"
#include "../Render/ShapeRenderer.h"

void TerrainParticleCreate(TerrainParticle * particle, Level * level, float x, float y, float z, float xd, float yd, float zd, Block * block) {
	ParticleCreate(particle, level, x, y, z, xd, yd, zd);
	ParticleData * this = &particle->particle;
	this->texture = block->textureID;
	this->gravity = block->particleGravity;
	this->r = 0.6;
	this->g = 0.6;
	this->b = 0.6;
	this->type = ParticleTypeTerrain;
}

int TerrainParticleGetTexture(TerrainParticle * particle) {
	return 1;
}

void TerrainParticleRender(TerrainParticle * particle, float dt, float x, float y, float z, float v6, float v7) {
	ParticleData * this = &particle->particle;
	float u0 = ((this->texture % 16) + this->u / 4.0) / 16.0;
	float v0 = ((this->texture / 16) + this->v / 4.0) / 16.0;
	float u1 = u0 + 0.015609375;
	float v1 = v0 + 0.015609375;
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
