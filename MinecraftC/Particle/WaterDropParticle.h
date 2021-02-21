#pragma once
#include "Particle.h"

typedef Particle WaterDropParticle;

WaterDropParticle WaterDropParticleCreate(struct Level * level, float3 pos);
void WaterDropParticleTick(WaterDropParticle particle);
