#include "WaterDropParticle.h"
#include "../Level/Level.h"

WaterDropParticle WaterDropParticleCreate(Level level, float3 pos)
{
	Particle particle = ParticleCreate(level, pos, zero3f);
	ParticleData this = particle->TypeData;
	this->Type = ParticleTypeWaterDrop;
	this->Delta.xz *= 0.3;
	this->Delta.y = RandomUniform() * 0.2 + 0.1;
	this->Color = one3f;
	this->Texture = 16;
	EntitySetSize(particle, 0.01, 0.01);
	this->LifeTime = 8.0 / (RandomUniform() * 0.8 + 0.2);
	return particle;
}

void WaterDropParticleTick(WaterDropParticle particle)
{
	ParticleData this = particle->TypeData;
	particle->OldPosition = particle->Position;
	this->Delta.y -= 0.06;
	EntityMove(particle, this->Delta);
	this->Delta *= 0.98;
	if (this->LifeTime-- <= 0) { EntityRemove(particle); }
	if (particle->OnGround)
	{
		if (RandomUniform() < 0.5) { EntityRemove(particle); }
		this->Delta.xz *= 0.7;
	}
}
