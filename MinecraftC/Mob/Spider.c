#include "Spider.h"
#include "AI/JumpAttackAI.h"

Spider SpiderCreate(Level level, float3 pos)
{
	Mob entity = MobCreate(level);
	entity->HeightOffset = 0.72;
	EntitySetSize(entity, 1.4, 0.9);
	EntitySetPosition(entity, pos);
	MobData mob = entity->TypeData;
	mob->Type = MobTypeSpider;
	mob->ModelName = "Spider";
	mob->TextureName = "Mob/Spider.png";
	mob->DeathScore = 105;
	mob->BobbingStrength = 0.0;
	AIDestroy(mob->AI);
	mob->AI = JumpAttackAICreate();
	return entity;
}
