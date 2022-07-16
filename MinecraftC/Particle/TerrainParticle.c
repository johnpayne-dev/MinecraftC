#include "TerrainParticle.h"
#include "../Level/Level.h"
#include "../Render/ShapeRenderer.h"

TerrainParticle TerrainParticleCreate(Level level, float3 pos, float3 vel, Block block) {
	Particle particle = ParticleCreate(level, pos, vel);
	ParticleData this = particle->typeData;
	this->texture = block->textureID;
	this->gravity = block->particleGravity;
	this->color = one3f * 0.6;
	this->type = ParticleTypeTerrain;
	return particle;
}

int TerrainParticleGetTexture(TerrainParticle particle) {
	return 1;
}

void TerrainParticleRender(TerrainParticle particle, float t, float3 v1, float v6, float v7) {
	ParticleData this = particle->typeData;
	float2 uv0 = (float2){ ((this->texture % 16) + this->uv.x / 4.0F) / 16.0F, ((this->texture / 16) + this->uv.y / 4.0F) / 16.0F };
	float2 uv1 = uv0 + 0.015609375;
	float s = 0.1 * this->size;
	float3 v = particle->oldPosition + (particle->position - particle->oldPosition) * t;
	float b = EntityGetBrightness(particle, t);
	ShapeRendererColor(this->color * b);
	ShapeRendererVertexUV(v + v1 * (float3){ -s, -s, -s } - (float3){ v6, 0.0, v7 } * s, (float2){ uv0.x, uv1.y });
	ShapeRendererVertexUV(v + v1 * (float3){ -s, s, -s } + (float3){ v6, 0.0, v7 } * s, (float2){ uv0.x, uv0.y });
	ShapeRendererVertexUV(v + v1 * (float3){ s, s, s } + (float3){ v6, 0.0, v7 } * s, (float2){ uv1.x, uv0.y });
	ShapeRendererVertexUV(v + v1 * (float3){ s, -s, s } - (float3){ v6, 0.0, v7 } * s, (float2){ uv1.x, uv1.y });
}
