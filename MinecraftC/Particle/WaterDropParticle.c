#include "WaterDropParticle.h"
#include "../Level/Level.h"

WaterDropParticle WaterDropParticleCreate(Level level, float3 pos) {
	Particle particle = ParticleCreate(level, pos, zero3f);
	ParticleData this = particle->typeData;
	this->type = ParticleTypeWaterDrop;
	this->delta.xz *= 0.3;
	this->delta.y = RandomUniform() * 0.2 + 0.1;
	this->color = one3f;
	this->texture = 16;
	EntitySetSize(particle, 0.01, 0.01);
	this->lifeTime = 8.0 / (RandomUniform() * 0.8 + 0.2);
	return particle;
}

void WaterDropParticleTick(WaterDropParticle particle) {
	ParticleData this = particle->typeData;
	particle->oldPosition = particle->position;
	this->delta.y -= 0.06;
	EntityMove(particle, this->delta);
	this->delta *= 0.98;
	if (this->lifeTime-- <= 0) { EntityRemove(particle); }
	if (particle->onGround)
	{
		if (RandomUniform() < 0.5) { EntityRemove(particle); }
		this->delta.xz *= 0.7;
	}
}
