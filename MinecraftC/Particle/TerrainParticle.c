#include "TerrainParticle.h"
#include "../Level/Level.h"
#include "../Render/ShapeRenderer.h"

TerrainParticle TerrainParticleCreate(Level level, float3 pos, float3 vel, Block block)
{
	Particle particle = ParticleCreate(level, pos, vel);
	ParticleData this = particle->TypeData;
	this->Texture = block->TextureID;
	this->Gravity = block->ParticleGravity;
	this->Color = one3f * 0.6;
	this->Type = ParticleTypeTerrain;
	return particle;
}

int TerrainParticleGetTexture(TerrainParticle particle)
{
	return 1;
}

void TerrainParticleRender(TerrainParticle particle, float t, float3 v1, float v6, float v7)
{
	ParticleData this = particle->TypeData;
	float2 uv0 = (float2){ ((this->Texture % 16) + this->UV.x / 4.0F) / 16.0F, ((this->Texture / 16) + this->UV.y / 4.0F) / 16.0F };
	float2 uv1 = uv0 + 0.015609375;
	float s = 0.1 * this->Size;
	float3 v = particle->OldPosition + (particle->Position - particle->OldPosition) * t;
	float b = EntityGetBrightness(particle, t);
	ShapeRendererColor(this->Color * b);
	ShapeRendererVertexUV(v + v1 * (float3){ -s, -s, -s } - (float3){ v6, 0.0, v7 } * s, (float2){ uv0.x, uv1.y });
	ShapeRendererVertexUV(v + v1 * (float3){ -s, s, -s } + (float3){ v6, 0.0, v7 } * s, (float2){ uv0.x, uv0.y });
	ShapeRendererVertexUV(v + v1 * (float3){ s, s, s } + (float3){ v6, 0.0, v7 } * s, (float2){ uv1.x, uv0.y });
	ShapeRendererVertexUV(v + v1 * (float3){ s, -s, s } - (float3){ v6, 0.0, v7 } * s, (float2){ uv1.x, uv1.y });
}
