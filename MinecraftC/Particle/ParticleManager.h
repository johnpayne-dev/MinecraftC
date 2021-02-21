#pragma once
#include "Particle.h"

typedef struct ParticleManager
{
	list(Particle) Particles[2];
	TextureManager Textures;
} * ParticleManager;

ParticleManager ParticleMangerCreate(struct Level * level, TextureManager textures);
void ParticleManagerSpawnParticle(ParticleManager particles, Particle particle);
void ParticleManagerTick(ParticleManager particles);
void ParticleManagerDestroy(ParticleManager particles);
