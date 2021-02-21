#include "BasicAI.h"
#include "BasicAttackAI.h"
#include "JumpAttackAI.h"
#include "../Mob.h"
#include "../CreeperAI.h"
#include "../SkeletonAI.h"
#include "../SheepAI.h"
#include "../../Player/PlayerAI.h"

BasicAI BasicAICreate()
{
	AI ai = AICreate();
	ai->Type = AITypeBasic;
	BasicAIData basicAI = MemoryAllocate(sizeof(struct BasicAIData));
	*basicAI = (struct BasicAIData)
	{
		.Random = RandomGeneratorCreate(TimeNano()),
		.Jumping = false,
		.AttackDelay = 0,
		.RunSpeed = 0.7,
		.NoActionTime = 0,
		.AttackTarget = NULL,
	};
	ai->TypeData = basicAI;
	return ai;
}

void BasicAITick(BasicAI ai, Level level, Entity mob)
{
	BasicAIData this = ai->TypeData;
	this->NoActionTime++;
	Entity player = LevelGetPlayer(level);
	if (this->NoActionTime > 600 && RandomGeneratorIntegerRange(this->Random, 0, 799) == 0 && player != NULL)
	{
		if (sqdistance3f(player->Position, mob->Position) < 1024.0) { this->NoActionTime = 0; }
		else { EntityRemove(mob); }
	}
	
	this->Level = level;
	this->Mob = mob;
	if (this->AttackDelay > 0) { this->AttackDelay--; }
	if (((MobData)mob->TypeData)->Health <= 0)
	{
		this->Jumping = false;
		this->XY = zero2f;
		this->Rotation = 0.0;
	}
	else { BasicAIUpdate(ai); }
	
	bool inWater = EntityIsInWater(mob);
	bool inLava = EntityIsInLava(mob);
	if (this->Jumping)
	{
		if (inWater) { mob->Delta.y += 0.04; }
		else if (inLava) { mob->Delta.y += 0.04; }
		else if (mob->OnGround) { BasicAIJumpFromGround(ai); }
	}
	
	this->XY *= 0.98;
	this->Rotation *= 0.9;
	MobTravel(mob, this->XY.x, this->XY.y);
	list(Entity) entities = LevelFindEntities(level, AABBGrow(mob->AABB, (float3){ 0.2, 0.0, 0.2 }));
	for (int i = 0; i < ListCount(entities); i++)
	{
		if (EntityIsPushable(entities[i])) { EntityPush(entities[i], mob); }
	}
	ListDestroy(entities);
	
	if (this->Type == BasicAITypeBasicAttack && ((BasicAttackAIData)this->TypeData)->Type == BasicAttackAITypeSkeleton) { SkeletonAITick(ai, level, mob); }
}

void BasicAIJumpFromGround(BasicAI ai)
{
	BasicAIData this = ai->TypeData;
	if (this->Type == BasicAITypeBasicAttack && ((BasicAttackAIData)this->TypeData)->Type == BasicAttackAIJumpAttack && this->AttackTarget != NULL) { return JumpAttackAIJumpFromGround(ai); }
	this->Mob->Delta.y = 0.42;
}

void BasicAIUpdate(BasicAI ai)
{
	BasicAIData this = ai->TypeData;
	if (this->Type == BasicAITypeSheep) { return SheepAIUpdate(ai); }
	if (this->Type == BasicAITypePlayer) { return PlayerAIUpdate(ai); }
	
	if (RandomGeneratorUniform(this->Random) < 0.07) { this->XY = (float2){ RandomGeneratorUniform(this->Random) - 0.5, RandomGeneratorUniform(this->Random) } * this->RunSpeed; }
	this->Jumping = RandomGeneratorUniform(this->Random) < 0.01;
	if (RandomGeneratorUniform(this->Random) < 0.04) { this->Rotation = (RandomGeneratorUniform(this->Random) - 0.5) * 60.0; }
	
	this->Mob->Rotation.y += this->Rotation;
	this->Mob->Rotation.x = ai->DefaultLookAngle;
	if (this->AttackTarget != NULL)
	{
		this->XY.y = this->RunSpeed;
		this->Jumping = RandomGeneratorUniform(this->Random) < 0.04;
	}
	if (EntityIsInWater(this->Mob) || EntityIsInLava(this->Mob)) { this->Jumping = RandomGeneratorUniform(this->Random) < 0.8; }
	
	if (this->Type == BasicAITypeBasicAttack) { BasicAttackAIUpdate(ai); }
}

void BasicAIBeforeRemove(BasicAI ai)
{
	BasicAIData this = ai->TypeData;
	if (this->Type == BasicAITypeBasicAttack && ((BasicAttackAIData)this->TypeData)->Type == BasicAttackAITypeCreeper) { return CreeperAIBeforeRemove(ai); }
	if (this->Type == BasicAITypeBasicAttack && ((BasicAttackAIData)this->TypeData)->Type == BasicAttackAITypeSkeleton) { return SkeletonAIBeforeRemove(ai); }
}

void BasicAIHurt(BasicAI ai, Entity entity, int damage)
{
	BasicAIData this = ai->TypeData;
	this->NoActionTime = 0;
	if (this->Type == BasicAITypeBasicAttack) { BasicAttackAIHurt(ai, entity, damage); }
}

void BasicAIDestroy(BasicAI ai)
{
	BasicAIData this = ai->TypeData;
	if (this->Type == BasicAITypeBasicAttack) { BasicAttackAIDestroy(ai); }
	if (this->Type == BasicAITypeSheep) { SheepAIDestroy(ai); }
	if (this->Type == BasicAITypePlayer) { PlayerAIDestroy(ai); }
	RandomGeneratorDestroy(this->Random);
	MemoryFree(this);
}
