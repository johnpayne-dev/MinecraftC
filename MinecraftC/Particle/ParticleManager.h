#pragma once
#include "Particle.h"

typedef struct ParticleManager {
	List(Particle) particles[2];
	TextureManager textures;
} * ParticleManager;

ParticleManager ParticleMangerCreate(struct Level * level, TextureManager textures);
void ParticleManagerSpawnParticle(ParticleManager particles, Particle particle);
void ParticleManagerTick(ParticleManager particles);
void ParticleManagerDestroy(ParticleManager particles);
