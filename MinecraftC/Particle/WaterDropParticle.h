#pragma once
#include "Particle.h"

typedef Particle WaterDropParticle;

WaterDropParticle WaterDropParticleCreate(struct Level * level, float x, float y, float z);
void WaterDropParticleTick(WaterDropParticle particle);
