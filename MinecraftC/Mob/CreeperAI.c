#include "CreeperAI.h"
#include "../Particle/TerrainParticle.h"

CreeperAI CreeperAICreate(Creeper parent)
{
	CreeperAI ai = BasicAttackAICreate();
	BasicAIData basicAI = ai->TypeData;
	BasicAttackAIData attackAI = basicAI->TypeData;
	attackAI->Type = BasicAttackAITypeCreeper;
	CreeperAIData creeper = MemoryAllocate(sizeof(struct CreeperAIData));
	*creeper = (struct CreeperAIData) { .Parent = parent, };
	attackAI->TypeData = creeper;
	return ai;
}

bool CreeperAIAttack(CreeperAI ai, Entity entity)
{
	BasicAIData basicAI = ai->TypeData;
	BasicAttackAIData attackAI = basicAI->TypeData;
	
	attackAI->Type = BasicAttackAITypeNone;
	if (!BasicAttackAIAttack(ai, entity))
	{
		attackAI->Type = BasicAttackAITypeCreeper;
		return false;
	}
	else
	{
		attackAI->Type = BasicAttackAITypeCreeper;
		EntityHurt(basicAI->Mob, entity, 6);
		return true;
	}
}

void CreeperAIBeforeRemove(CreeperAI ai)
{
	BasicAIData basicAI = ai->TypeData;
	
	LevelExplode(basicAI->Level, basicAI->Mob, basicAI->Mob->Position, 4.0);
	for (int i = 0; i < 500; i++)
	{
		float3 v = { RandomGeneratorNormal(basicAI->Random, 1.0), RandomGeneratorNormal(basicAI->Random, 1.0), RandomGeneratorNormal(basicAI->Random, 1.0) };
		float3 w = v / length3f(v);
		ParticleManagerSpawnParticle(basicAI->Level->ParticleEngine, TerrainParticleCreate(basicAI->Level, basicAI->Mob->Position + v, w, Blocks.Table[BlockTypeLeaves]));
	}
}

void CreeperAIDestroy(CreeperAI ai)
{
	BasicAIData basicAI = ai->TypeData;
	BasicAttackAIData attackAI = basicAI->TypeData;
	CreeperAIData this = attackAI->TypeData;
	MemoryFree(this);
}
