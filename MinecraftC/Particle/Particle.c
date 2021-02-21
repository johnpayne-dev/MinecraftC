#include "Particle.h"
#include "SmokeParticle.h"
#include "TerrainParticle.h"
#include "WaterDropParticle.h"
#include "../Level/Level.h"
#include "../Render/ShapeRenderer.h"

Particle ParticleCreate(Level level, float3 pos, float3 vel)
{
	Entity entity = EntityCreate(level);
	entity->Type = EntityTypeParticle;
	EntitySetSize(entity, 0.2, 0.2);
	entity->HeightOffset = entity->AABBHeight / 2.0;
	EntitySetPosition(entity, pos);
	entity->MakeStepSound = false;
	ParticleData this = MemoryAllocate(sizeof(struct ParticleData));
	*this = (struct ParticleData)
	{
		.Color = one3f,
		.Delta = vel + (RandomUniform() * 2.0 - 1.0) * 0.4,
		.UV = (float2){ RandomUniform(), RandomUniform() } * 3.0,
		.Size = RandomUniform() * 0.5 + 0.5,
		.LifeTime = 4.0 / (RandomUniform() * 0.9 + 0.1),
		.Age = 0,
		
	};
	entity->TypeData = this;
	this->Delta *= (RandomUniform() + RandomUniform() + 1.0) * 0.15 * 0.4 / length3f(this->Delta);
	this->Delta.y += 0.1;
	return entity;
}

Particle ParticleSetPower(Particle particle, float power)
{
	ParticleData this = particle->TypeData;
	this->Delta.xz *= power;
	this->Delta.y = (this->Delta.y - 0.1) * power + 0.1;
	return particle;
}

Particle ParticleScale(Particle particle, float scale)
{
	ParticleData this = particle->TypeData;
	EntitySetSize(particle, 0.2 * scale, 0.2 * scale);
	this->Size *= scale;
	return particle;
}

void ParticleTick(Particle particle)
{
	ParticleData this = particle->TypeData;
	if (this->Type == ParticleTypeSmoke) { SmokeParticleTick(particle); }
	if (this->Type == ParticleTypeWaterDrop) { WaterDropParticleTick(particle); }
	
	particle->OldPosition = particle->Position;
	if (this->Age++ >= this->LifeTime) { EntityRemove(particle); }
	this->Delta.y -= 0.04 * this->Gravity;
	EntityMove(particle, this->Delta);
	this->Delta *= 0.98;
	if (particle->OnGround) { this->Delta.xz *= 0.7; }
}

void ParticleRender(Particle particle, float t, float3 v1, float v6, float v7)
{
	ParticleData this = particle->TypeData;
	if (this->Type == ParticleTypeTerrain) { return TerrainParticleRender(particle, t, v1, v6, v7); }

	float2 uv0 = { ((this->Texture % 16) + this->UV.x / 4.0F) / 16.0F, ((this->Texture / 16) + this->UV.y / 4.0F) / 16.0F };
	float2 uv1 = uv0 + 0.0624375;
	float s = 0.1 * this->Size;
	float3 v = particle->OldPosition + (particle->Position - particle->OldPosition) * t;
	float b = EntityGetBrightness(particle, t);
	ShapeRendererColor(this->Color * b);
	ShapeRendererVertexUV(v + v1 * (float3){ -s, -s, -s } - (float3){ v6, 0.0, v7 } * s, (float2){ uv0.x, uv1.y });
	ShapeRendererVertexUV(v + v1 * (float3){ -s, s, -s } + (float3){ v6, 0.0, v7 } * s, (float2){ uv0.x, uv0.y });
	ShapeRendererVertexUV(v + v1 * (float3){ s, s, s } + (float3){ v6, 0.0, v7 } * s, (float2){ uv1.x, uv0.y });
	ShapeRendererVertexUV(v + v1 * (float3){ s, -s, s } - (float3){ v6, 0.0, v7 } * s, (float2){ uv1.x, uv1.y });
}

int ParticleGetParticleTexture(Particle particle)
{
	ParticleData this = particle->TypeData;
	if (this->Type == ParticleTypeTerrain) { return TerrainParticleGetTexture(particle); }
	return 0;
}

void ParticleDestroy(Particle particle)
{
	ParticleData this = particle->TypeData;
	MemoryFree(this);
}
