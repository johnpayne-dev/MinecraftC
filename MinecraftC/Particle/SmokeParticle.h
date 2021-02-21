#pragma once
#include "Particle.h"

typedef Particle SmokeParticle;

SmokeParticle SmokeParticleCreate(struct Level * level, float3 pos);
void SmokeParticleTick(SmokeParticle particle);
