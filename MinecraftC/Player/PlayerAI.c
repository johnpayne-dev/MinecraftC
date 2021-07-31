#include "PlayerAI.h"
#include "Player.h"

PlayerAI PlayerAICreate(Player parent)
{
	PlayerAI ai = MemoryAllocate(sizeof(struct PlayerAI));
	*ai = (struct PlayerAI)
	{
		.DefaultLookAngle = 0,
		.Random = RandomGeneratorCreate(TimeNano()),
		.Jumping = false,
		.AttackDelay = 0,
		.RunSpeed = 0.7,
		.NoActionTime = 0,
		.AttackTarget = NULL,
		.Parent = parent,
	};
	return ai;
}

void PlayerAIUpdate(PlayerAI ai)
{
	PlayerData player = ai->Parent->TypeData;
	ai->Jumping = player->Input->Jumping;
	ai->XY = player->Input->XY;
}

void PlayerAITick(PlayerAI ai, Level level, Entity mob)
{
	ai->NoActionTime++;
	Entity player = LevelGetPlayer(level);
	if (ai->NoActionTime > 600 && RandomGeneratorIntegerRange(ai->Random, 0, 799) == 0 && player != NULL)
	{
		if (sqdistance3f(player->Position, mob->Position) < 1024.0) { ai->NoActionTime = 0; }
		else { EntityRemove(mob); }
	}
	
	ai->Level = level;
	ai->Mob = mob;
	if (ai->AttackDelay > 0) { ai->AttackDelay--; }
	PlayerAIUpdate(ai);
	
	bool inWater = EntityIsInWater(mob);
	bool inLava = EntityIsInLava(mob);
	if (ai->Jumping)
	{
		if (inWater) { mob->Delta.y += 0.04; }
		else if (inLava) { mob->Delta.y += 0.04; }
		else if (mob->OnGround) { PlayerAIJumpFromGround(ai); }
	}
	
	ai->XY *= 0.98;
	ai->Rotation *= 0.9;
	PlayerTravel(mob, ai->XY.x, ai->XY.y);
}

void PlayerAIJumpFromGround(PlayerAI ai)
{
	ai->Mob->Delta.y = 0.42;
}

void PlayerAIDestroy(PlayerAI ai)
{
	RandomGeneratorDestroy(ai->Random);
	MemoryFree(ai);
}

