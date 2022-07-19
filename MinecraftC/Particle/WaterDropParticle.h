#pragma once
#include "Particle.h"

typedef Particle WaterDropParticle;

void WaterDropParticleCreate(WaterDropParticle * particle, struct Level * level, float x, float y, float z);
void WaterDropParticleTick(WaterDropParticle * particle);
