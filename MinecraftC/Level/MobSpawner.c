#include "MobSpawner.h"
#include "../Mob/Mob.h"
#include "../Mob/Zombie.h"
#include "../Mob/Skeleton.h"
#include "../Mob/Pig.h"
#include "../Mob/Creeper.h"
#include "../Mob/Spider.h"
#include "../Mob/Sheep.h"

MobSpawner MobSpawnerCreate(Level level)
{
	MobSpawner spawner = MemoryAllocate(sizeof(struct MobSpawner));
	*spawner = (struct MobSpawner){ .Level = level, };
	return spawner;
}

int MobSpawnerSpawn(MobSpawner spawner, int count, Entity entity, ProgressBarDisplay progressBar)
{
	int spawned = 0;
	for (int i = 0; i < count; i++)
	{
		if (progressBar != NULL) { ProgressBarDisplaySetProgress(progressBar, i * 100 / (count - 1)); }
		
		int3 v = { 0 };
		v.x = (int)RandomGeneratorIntegerRange(spawner->Level->Random, 0, spawner->Level->Width - 1);
		v.y = fmin(RandomGeneratorUniform(spawner->Level->Random), RandomGeneratorUniform(spawner->Level->Random)) * spawner->Level->Depth;
		v.z = (int)RandomGeneratorIntegerRange(spawner->Level->Random, 0, spawner->Level->Height - 1);
		int mobNum = (int)RandomGeneratorIntegerRange(spawner->Level->Random, 0, 5);
		if (!LevelIsSolidTile(spawner->Level, v.x, v.y, v.z) && LevelGetLiquidType(spawner->Level, v.x, v.y, v.z) == LiquidTypeNone && (!LevelIsLit(spawner->Level, v.x, v.y, v.z) || RandomGeneratorIntegerRange(spawner->Level->Random, 0, 5) == 0))
		{
			for (int j = 0; j < 3; j++)
			{
				int3 w = v;
				for (int k = 0; k < 3; k++)
				{
					w.x += RandomGeneratorIntegerRange(spawner->Level->Random, 0, 5) - RandomGeneratorIntegerRange(spawner->Level->Random, 0, 5);
					w.y += RandomGeneratorIntegerRange(spawner->Level->Random, 0, 0) - RandomGeneratorIntegerRange(spawner->Level->Random, 0, 0);
					w.z += RandomGeneratorIntegerRange(spawner->Level->Random, 0, 5) - RandomGeneratorIntegerRange(spawner->Level->Random, 0, 5);
					if (w.x >= 0 && w.z >= 1 && w.y >= 0 && w.y < spawner->Level->Depth - 2 && w.x < spawner->Level->Width && w.z < spawner->Level->Height && LevelIsSolidTile(spawner->Level, w.x, w.y - 1, w.z) && !LevelIsSolidTile(spawner->Level, w.x, w.y, w.z) && !LevelIsSolidTile(spawner->Level, w.x, w.y + 1, w.z))
					{
						float3 wf = float3i(w) + (float3){ 0.5, 1.0, 0.5 };
						if (entity != NULL && sqdistance3f(wf, entity->Position) < 256.0) { continue; }
						else if (sqdistance3f(wf, float3i(spawner->Level->Spawn)) < 256.0) { continue;}
						
						Mob mob = NULL;
						if (mobNum == 0) { mob = ZombieCreate(spawner->Level, wf); }
						if (mobNum == 1) { mob = SkeletonCreate(spawner->Level, wf); }
						if (mobNum == 2) { mob = PigCreate(spawner->Level, wf); }
						if (mobNum == 3) { mob = CreeperCreate(spawner->Level, wf); }
						if (mobNum == 4) { mob = SpiderCreate(spawner->Level, wf); }
						if (mobNum == 5) { mob = SheepCreate(spawner->Level, wf); }
						if (LevelIsFree(spawner->Level, mob->AABB)) { spawned++; LevelAddEntity(spawner->Level, mob); }
					}
				}
			}
		}
	}
	return spawned;
}

void MobSpawnerDestroy(MobSpawner spawner)
{
	MemoryFree(spawner);
}
