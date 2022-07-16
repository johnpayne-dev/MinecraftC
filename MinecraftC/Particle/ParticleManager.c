#include "ParticleManager.h"
#include "../Level/Level.h"

ParticleManager ParticleMangerCreate(Level level, TextureManager textures) {
	ParticleManager manager = MemoryAllocate(sizeof(struct ParticleManager));
	*manager = (struct ParticleManager) {
		.textures = textures,
	};
	if (level != NULL) { level->particleEngine = manager; }
	for (int i = 0; i < 2; i++) { manager->particles[i] = ListCreate(sizeof(Particle)); }
	return manager;
}

void ParticleManagerSpawnParticle(ParticleManager manager, Particle particle) {
	int tex = ParticleGetParticleTexture(particle);
	manager->particles[tex] = ListPush(manager->particles[tex], &particle);
}

void ParticleManagerTick(ParticleManager manager) {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < ListCount(manager->particles[i]); j++) {
			ParticleTick(manager->particles[i][j]);
			if (manager->particles[i][j]->removed) {
				ParticleDestroy(manager->particles[i][j]);
				manager->particles[i] = ListRemove(manager->particles[i], j--);
			}
		}
	}
}

void ParticleManagerDestroy(ParticleManager manager) {
	for (int i = 0; i < 2; i++) { ListDestroy(manager->particles[i]); }
	MemoryFree(manager);
}
