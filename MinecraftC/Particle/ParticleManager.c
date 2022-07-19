#include "ParticleManager.h"
#include "../Level/Level.h"

void ParticleMangerCreate(ParticleManager * particles, Level * level, TextureManager * textures) {
	*particles = (ParticleManager) {
		.textures = textures,
	};
	if (level != NULL) { level->particleEngine = particles; }
	for (int i = 0; i < 2; i++) { particles->particles[i] = ListCreate(sizeof(Particle *)); }
}

void ParticleManagerSpawnParticle(ParticleManager * manager, Particle * particle) {
	int tex = ParticleGetParticleTexture(particle);
	manager->particles[tex] = ListPush(manager->particles[tex], &particle);
}

void ParticleManagerTick(ParticleManager * manager) {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < ListLength(manager->particles[i]); j++) {
			ParticleTick(manager->particles[i][j]);
			if (manager->particles[i][j]->removed) {
				manager->particles[i] = ListRemove(manager->particles[i], j--);
			}
		}
	}
}

void ParticleManagerDestroy(ParticleManager * manager) {
	for (int i = 0; i < 2; i++) { ListFree(manager->particles[i]); }
}
