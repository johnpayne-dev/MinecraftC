#include "PlayerAI.h"

PlayerAI PlayerAICreate(Player parent)
{
	AI ai = BasicAICreate();
	BasicAIData basicAI = ai->TypeData;
	basicAI->Type = BasicAITypePlayer;
	PlayerAIData this = MemoryAllocate(sizeof(struct PlayerAIData));
	*this = (struct PlayerAIData)
	{
		.Parent = parent,
	};
	basicAI->TypeData = this;
	return ai;
}

void PlayerAIUpdate(PlayerAI ai)
{
	BasicAIData basicAI = ai->TypeData;
	PlayerAIData this = basicAI->TypeData;
	PlayerData player = ((MobData)this->Parent->TypeData)->TypeData;
	basicAI->Jumping = player->Input->Jumping;
	basicAI->XY = player->Input->XY;
}

void PlayerAIDestroy(PlayerAI ai)
{
	BasicAIData basicAI = ai->TypeData;
	PlayerAIData this = basicAI->TypeData;
	MemoryFree(this);
}
