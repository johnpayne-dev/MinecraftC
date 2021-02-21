#include "BasicAttackAI.h"
#include "../Mob.h"
#include "../CreeperAI.h"
#include "../SkeletonAI.h"
#include "../../Item/Arrow.h"

BasicAttackAI BasicAttackAICreate(void)
{
	BasicAI ai = BasicAICreate();
	BasicAIData basicAI = ai->TypeData;
	basicAI->Type = BasicAITypeBasicAttack;
	BasicAttackAIData attackAI = MemoryAllocate(sizeof(struct BasicAttackAIData));
	*attackAI = (struct BasicAttackAIData){ .Damage = 6, };
	basicAI->TypeData = attackAI;
	return ai;
}

void BasicAttackAIUpdate(BasicAttackAI ai)
{
	BasicAIData basicAI = ai->TypeData;
	if (((MobData)basicAI->Mob->TypeData)->Health > 0) { BasicAttackAIDoAttack(ai); }
}

void BasicAttackAIDoAttack(BasicAttackAI ai)
{
	BasicAIData basicAI = ai->TypeData;
	
	Entity player = LevelGetPlayer(basicAI->Level);
	if (basicAI->AttackTarget != NULL && basicAI->AttackTarget->Removed) { basicAI->AttackTarget = NULL; }
	
	if (player != NULL && basicAI->AttackTarget == NULL)
	{
		if (sqdistance3f(player->Position, basicAI->Mob->Position) < 16.0 * 16.0) { basicAI->AttackTarget = player; }
	}
	
	if (basicAI->AttackTarget != NULL)
	{
		if (sqdistance3f(basicAI->AttackTarget->Position, basicAI->Mob->Position) > 16.0 * 16.0 * 4.0 && RandomGeneratorIntegerRange(basicAI->Random, 0, 99) == 0) { basicAI->AttackTarget = NULL; }
		if (basicAI->AttackTarget != NULL)
		{
			float3 d = basicAI->AttackTarget->Position - basicAI->Mob->Position;
			float dist = distance3f(basicAI->AttackTarget->Position, basicAI->Mob->Position);
			basicAI->Mob->Rotation.y = atan2(d.z, d.x) * deg - 90.0;
			basicAI->Mob->Rotation.x = -atan2(d.y, dist) * deg;
			if (dist < 2.0 && basicAI->AttackDelay == 0) { BasicAttackAIAttack(ai, basicAI->AttackTarget); }
		}
	}
}

bool BasicAttackAIAttack(BasicAttackAI ai, Entity entity)
{
	BasicAIData basicAI = ai->TypeData;
	BasicAttackAIData this = basicAI->TypeData;
	if (this->Type == BasicAttackAITypeCreeper) { return CreeperAIAttack(ai, entity); }
	
	if (!LevelClip(basicAI->Level, basicAI->Mob->Position, entity->Position).Null) { return false; }
	else
	{
		((MobData)basicAI->Mob->TypeData)->AttackTime = 5;
		basicAI->AttackDelay = (int)RandomGeneratorIntegerRange(basicAI->Random, 10, 30);
		int damage = (RandomGeneratorUniform(basicAI->Random) + RandomGeneratorUniform(basicAI->Random)) / 2.0 * this->Damage + 1.0;
		EntityHurt(entity, basicAI->Mob, damage);
		basicAI->NoActionTime = 0;
		return true;
	}
}

void BasicAttackAIHurt(BasicAttackAI ai, Entity entity, int var)
{
	BasicAIData basicAI = ai->TypeData;
	if (entity != NULL && entity->Type == EntityTypeArrow) { entity = ArrowGetOwner(entity); }
	if (entity != NULL && entity->Type != basicAI->Mob->Type) { basicAI->AttackTarget = entity; }
}

void BasicAttackAIDestroy(BasicAttackAI ai)
{
	BasicAIData basicAI = ai->TypeData;
	BasicAttackAIData this = basicAI->TypeData;
	if (this->Type == BasicAttackAITypeCreeper) { CreeperAIDestroy(ai); }
	if (this->Type == BasicAttackAITypeSkeleton) { SkeletonAIDestroy(ai); }
	MemoryFree(this);
}
