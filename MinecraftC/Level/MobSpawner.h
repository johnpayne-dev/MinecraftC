#pragma once
#include "Level.h"
#include "../ProgressBarDisplay.h"

typedef struct MobSpawner
{
	Level Level;
} * MobSpawner;

MobSpawner MobSpawnerCreate(Level level);
int MobSpawnerSpawn(MobSpawner spawner, int count, Entity entity, ProgressBarDisplay progressBar);
void MobSpawnerDestroy(MobSpawner spawner);
