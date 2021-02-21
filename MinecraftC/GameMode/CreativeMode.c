#include "CreativeMode.h"
#include "../Minecraft.h"
#include "../GUI/BlockSelectScreen.h"

CreativeMode CreativeModeCreate(Minecraft minecraft)
{
	GameMode mode = GameModeCreate(minecraft);
	mode->Type = GameModeTypeCreative;
	mode->InstantBreak = true;
	return mode;
}

void CreativeModeOpenInventory(CreativeMode mode)
{
	MinecraftSetCurrentScreen(mode->Minecraft, BlockSelectScreenCreate());
}

void CreativeModeApply(CreativeMode mode, Level level)
{
	LevelRemoveAllNonCreativeModeEntities(level);
	level->CreativeMode = true;
	level->GrowTrees = false;
}

void CreativeModeApplyPlayer(CreativeMode mode, Player player)
{
	MobData playerMob = player->TypeData;
	PlayerData playerData = playerMob->TypeData;
	for (int i = 0; i < 9; i++)
	{
		playerData->Inventory->Count[i] = 1;
		if (playerData->Inventory->Slots[i] == -1)
		{
			playerData->Inventory->Slots[i] = SessionDataAllowedBlocks[i]->Type;
		}
	}
}

bool CreativeModeIsSurvival(CreativeMode mode)
{
	return false;
}
