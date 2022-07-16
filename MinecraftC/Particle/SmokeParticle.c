#include "SmokeParticle.h"
#include "../Level/Level.h"

SmokeParticle SmokeParticleCreate(Level level, float3 pos) {
	SmokeParticle particle = ParticleCreate(level, pos, zero3f);
	ParticleData this = particle->typeData;
	this->type = ParticleTypeSmoke;
	this->delta *= 0.1;
	this->color = one3f * RandomUniform() * 0.3;
	this->lifeTime = 8.0 / (RandomUniform() * 0.8 + 0.2);
	particle->noPhysics = true;
	return particle;
}

void SmokeParticleTick(SmokeParticle particle) {
	ParticleData this = particle->typeData;
	particle->oldPosition = particle->position;
	if (this->age++ >= this->lifeTime) { EntityRemove(particle); }
	this->texture = 7 - (this->age << 3) / this->lifeTime;
	this->delta.y += 0.004;
	EntityMove(particle, this->delta);
	this->delta *= 0.96;
	if (particle->onGround) { this->delta *= 0.7; }
}
