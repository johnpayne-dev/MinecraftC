#include "SmokeParticle.h"
#include "../Level/Level.h"

void SmokeParticleCreate(SmokeParticle * particle, Level * level, float x, float y, float z) {
	ParticleCreate(particle, level, x, y, z, 0, 0, 0);
	ParticleData * this = &particle->particle;
	this->type = ParticleTypeSmoke;
	this->xd *= 0.1;
	this->yd *= 0.1;
	this->zd *= 0.1;
	this->r = RandomUniform() * 0.3;
	this->g = this->r;
	this->b = this->r;
	this->lifeTime = 8.0 / (RandomUniform() * 0.8 + 0.2);
	particle->noPhysics = true;
}

void SmokeParticleTick(SmokeParticle * particle) {
	ParticleData * this = &particle->particle;
	particle->xo = particle->x;
	particle->yo = particle->y;
	particle->zo = particle->z;
	if (this->age++ >= this->lifeTime) { EntityRemove(particle); }
	this->texture = 7 - (this->age << 3) / this->lifeTime;
	this->yd += 0.004;
	EntityMove(particle, this->xd, this->yd, this->zd);
	this->xd *= 0.96;
	this->yd *= 0.96;
	this->zd *= 0.96;
	if (particle->onGround) {
		this->xd *= 0.7;
		this->yd *= 0.7;
		this->zd *= 0.7;
	}
}
