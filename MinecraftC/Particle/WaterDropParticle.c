#include "WaterDropParticle.h"
#include "../Level/Level.h"

void WaterDropParticleCreate(WaterDropParticle * particle, Level * level, float x, float y, float z) {
	ParticleCreate(particle, level, x, y, z, 0.0, 0.0, 0.0);
	ParticleData * this = &particle->particle;
	this->type = ParticleTypeWaterDrop;
	this->xd *= 0.3;
	this->zd *= 0.3;
	this->yd = RandomUniform() * 0.2 + 0.1;
	this->r = 1.0;
	this->g = 1.0;
	this->b = 1.0;
	this->texture = 16;
	EntitySetSize(particle, 0.01, 0.01);
	this->lifeTime = 8.0 / (RandomUniform() * 0.8 + 0.2);
}

void WaterDropParticleTick(WaterDropParticle * particle) {
	ParticleData * this = &particle->particle;
	particle->xo = particle->x;
	particle->yo = particle->y;
	particle->zo = particle->z;
	this->yd -= 0.06;
	EntityMove(particle, this->xd, this->yd, this->zd);
	this->xd *= 0.98;
	this->yd *= 0.98;
	this->zd *= 0.98;
	if (this->lifeTime-- <= 0) { EntityRemove(particle); }
	if (particle->onGround) {
		if (RandomUniform() < 0.5) { EntityRemove(particle); }
		this->xd *= 0.7;
		this->zd *= 0.7;
	}
}
