#pragma once
#include "Particle.h"

typedef Particle SmokeParticle;

SmokeParticle SmokeParticleCreate(struct Level * level, float x, float y, float z);
void SmokeParticleTick(SmokeParticle particle);
