#pragma once
#include "Particle.h"

typedef Particle SmokeParticle;

void SmokeParticleCreate(SmokeParticle * particle, struct Level * level, float x, float y, float z);
void SmokeParticleTick(SmokeParticle * particle);
