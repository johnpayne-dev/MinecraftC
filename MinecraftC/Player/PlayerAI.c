#include "PlayerAI.h"
#include "Player.h"
#include "../Entity.h"
#include <stdlib.h>

void PlayerAICreate(PlayerAI * ai, Player * parent) {
	*ai = (PlayerAI) {
		.jumping = false,
		.parent = parent,
	};
	RandomGeneratorCreate(&ai->random, TimeNano());
}

void PlayerAIUpdate(PlayerAI * ai) {
	PlayerData * player = &ai->parent->player;
	ai->jumping = player->input.jumping;
	ai->x = player->input.x;
	ai->y = player->input.y;
}

void PlayerAITick(PlayerAI * ai, Entity * mob) {
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

void PlayerAIJumpFromGround(PlayerAI * ai) {
	ai->mob->yd = 0.42;
}
