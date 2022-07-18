#include "PlayerAI.h"
#include "Player.h"

PlayerAI PlayerAICreate(Player parent) {
	PlayerAI ai = MemoryAllocate(sizeof(struct PlayerAI));
	*ai = (struct PlayerAI) {
		.random = RandomGeneratorCreate(TimeNano()),
		.jumping = false,
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

void PlayerAITick(PlayerAI ai, Entity mob) {
	ai->mob = mob;
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
	PlayerTravel(mob, ai->x, ai->y);
}

void PlayerAIJumpFromGround(PlayerAI ai) {
	ai->mob->yd = 0.42;
}

void PlayerAIDestroy(PlayerAI ai) {
	RandomGeneratorDestroy(ai->random);
	MemoryFree(ai);
}

