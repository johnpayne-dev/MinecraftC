#include "SmokeParticle.h"
#include "../Level/Level.h"

SmokeParticle SmokeParticleCreate(Level level, float3 pos)
{
	SmokeParticle particle = ParticleCreate(level, pos, zero3f);
	ParticleData this = particle->TypeData;
	this->Type = ParticleTypeSmoke;
	this->Delta *= 0.1;
	this->Color = one3f * RandomUniform() * 0.3;
	this->LifeTime = 8.0 / (RandomUniform() * 0.8 + 0.2);
	particle->NoPhysics = true;
	return particle;
}

void SmokeParticleTick(SmokeParticle particle)
{
	ParticleData this = particle->TypeData;
	particle->OldPosition = particle->Position;
	if (this->Age++ >= this->LifeTime) { EntityRemove(particle); }
	this->Texture = 7 - (this->Age << 3) / this->LifeTime;
	this->Delta.y += 0.004;
	EntityMove(particle, this->Delta);
	this->Delta *= 0.96;
	if (particle->OnGround) { this->Delta *= 0.7; }
}
