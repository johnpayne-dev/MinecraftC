#pragma once
#include "../Utilities/List.h"
#include "../Render/TextureManager.h"
#include "Particle.h"

typedef struct ParticleManager {
	List(Particle *) particles[2];
	TextureManager * textures;
} ParticleManager;

void ParticleMangerCreate(ParticleManager * particles, struct Level * level, TextureManager * textures);
void ParticleManagerSpawnParticle(ParticleManager * particles, Particle * particle);
void ParticleManagerTick(ParticleManager * particles);
void ParticleManagerDestroy(ParticleManager * particles);
