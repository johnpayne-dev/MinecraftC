#include "ParticleManager.h"
#include "../Level/Level.h"

ParticleManager ParticleMangerCreate(Level level, TextureManager textures)
{
	ParticleManager manager = MemoryAllocate(sizeof(struct ParticleManager));
	*manager = (struct ParticleManager)
	{
		.Textures = textures,
	};
	if (level != NULL) { level->ParticleEngine = manager; }
	for (int i = 0; i < 2; i++) { manager->Particles[i] = ListCreate(sizeof(Particle)); }
	return manager;
}

void ParticleManagerSpawnParticle(ParticleManager manager, Particle particle)
{
	int tex = ParticleGetParticleTexture(particle);
	manager->Particles[tex] = ListPush(manager->Particles[tex], &particle);
}

void ParticleManagerTick(ParticleManager manager)
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < ListCount(manager->Particles[i]); j++)
		{
			ParticleTick(manager->Particles[i][j]);
			if (manager->Particles[i][j]->Removed)
			{
				ParticleDestroy(manager->Particles[i][j]);
				manager->Particles[i] = ListRemove(manager->Particles[i], j--);
			}
		}
	}
}

void ParticleManagerDestroy(ParticleManager manager)
{
	for (int i = 0; i < 2; i++) { ListDestroy(manager->Particles[i]); }
	MemoryFree(manager);
}
