#include "PlayerAI.h"
#include "Player.h"

PlayerAI PlayerAICreate(Player parent) {
	PlayerAI ai = MemoryAllocate(sizeof(struct PlayerAI));
	*ai = (struct PlayerAI) {
		.defaultLookAngle = 0,
		.random = RandomGeneratorCreate(TimeNano()),
		.jumping = false,
		.attackDelay = 0,
		.runSpeed = 0.7,
		.noActionTime = 0,
		.attackTarget = NULL,
		.parent = parent,
	};
	return ai;
}

void PlayerAIUpdate(PlayerAI ai) {
	PlayerData player = ai->parent->typeData;
	ai->jumping = player->input->jumping;
	ai->x = player->input->x;
	ai->y = player->input->y;
}

void PlayerAITick(PlayerAI ai, Level level, Entity mob) {
	ai->noActionTime++;
	Entity player = LevelGetPlayer(level);
	if (ai->noActionTime > 600 && RandomGeneratorIntegerRange(ai->random, 0, 799) == 0 && player != NULL) {
		float dx = mob->x - player->x;
		float dy = mob->y - player->y;
		float dz = mob->z - player->z;
		if (dx * dx + dy * dy + dz * dz < 1024.0) { ai->noActionTime = 0; }
		else { EntityRemove(mob); }
	}
	
	ai->level = level;
	ai->mob = mob;
	if (ai->attackDelay > 0) { ai->attackDelay--; }
	PlayerAIUpdate(ai);
	
	bool inWater = EntityIsInWater(mob);
	bool inLava = EntityIsInLava(mob);
	if (ai->jumping) {
		if (inWater) { mob->yd += 0.04; }
		else if (inLava) { mob->yd += 0.04; }
		else if (mob->onGround) { PlayerAIJumpFromGround(ai); }
	}
	
	ai->x *= 0.98;
	ai->y *= 0.98;
	ai->rotation *= 0.9;
	PlayerTravel(mob, ai->x, ai->y);
}

void PlayerAIJumpFromGround(PlayerAI ai) {
	ai->mob->yd = 0.42;
}

void PlayerAIDestroy(PlayerAI ai) {
	RandomGeneratorDestroy(ai->random);
	MemoryFree(ai);
}

