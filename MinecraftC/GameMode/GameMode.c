#include <string.h>
#include "GameMode.h"
#include "CreativeMode.h"
#include "SurvivalMode.h"
#include "../Minecraft.h"

GameMode GameModeCreate(Minecraft minecraft)
{
	GameMode mode = malloc(sizeof(struct GameMode));
	mode->Minecraft = minecraft;
	mode->InstantBreak = false;
	mode->Type = GameModeTypeNone;
	return mode;
}

void GameModeApply(GameMode mode, Level level)
{
	level->CreativeMode = false;
	level->GrowTrees = true;
	if (mode->Type == GameModeTypeCreative) { CreativeModeApply(mode, level); }
	if (mode->Type == GameModeTypeSurvival) { SurvivalModeApply(mode, level); }
}

void GameModeOpenInventory(GameMode mode)
{
	if (mode->Type == GameModeTypeCreative) { return CreativeModeOpenInventory(mode); }
}

void GameModeHitBlock(GameMode mode, int x, int y, int z)
{
	if (mode->Type == GameModeTypeSurvival) { return SurvivalModeHitBlock(mode, x, y, z); }
	GameModeBreakBlock(mode, x, y, z);
}

bool GameModeCanPlace(GameMode mode, BlockType blockType)
{
	if (mode->Type == GameModeTypeSurvival) { return SurvivalModeCanPlace(mode, blockType); }
	return true;
}

void GameModeBreakBlock(GameMode mode, int x, int y, int z)
{
	if (mode->Type == GameModeTypeSurvival) { SurvivalModeBreakBlock(mode, x, y, z); }
	
	Level level = mode->Minecraft->Level;
	Block block = Blocks.Table[LevelGetTile(level, x, y, z)];
	bool setTile = LevelNetSetTile(level, x, y, z, 0);
	if (block != NULL && setTile)
	{
		if (MinecraftIsOnline(mode->Minecraft))
		{
			//NetworkManagerSendBlockChange(mode->Minecraft->NetworkManager, x, y, z, 0, InventoryGetSelected(((PlayerData)mode->Minecraft->Player->TypeData)->Inventory));
		}
		
		if (block->Sound.Type != TileSoundTypeNone)
		{
			LevelPlaySoundAt(level, "step.wtf", (float3){ x, y, z}, (TileSoundGetVolume(block->Sound) + 1.0) / 2.0, TileSoundGetPitch(block->Sound) * 0.8);
		}
		
		BlockSpawnBreakParticles(block, level, x, y, z, mode->Minecraft->ParticleManager);
	}
}

void GameModeHitBlockSide(GameMode mode, int x, int y, int z, int side)
{
	if (mode->Type == GameModeTypeSurvival) { return SurvivalModeHitBlockSide(mode, x, y, z, side); }
}

void GameModeResetHits(GameMode mode)
{
	if (mode->Type == GameModeTypeSurvival) { return SurvivalModeResetHits(mode); }
}

void GameModeApplyCracks(GameMode mode, float cracks)
{
	if (mode->Type == GameModeTypeSurvival) { return SurvivalModeApplyCracks(mode, cracks); }
}

float GameModeGetReachDistance(GameMode mode)
{
	if (mode->Type == GameModeTypeSurvival) { return SurvivalModeGetReachDistance(mode); }
	return 5.0;
}

bool GameModeUseItem(GameMode mode, Player player, BlockType blockType)
{
	if (mode->Type == GameModeTypeSurvival) { return SurvivalModeUseItem(mode, player, blockType); }
	return false;
}

void GameModePreparePlayer(GameMode mode, Player player)
{
	if (mode->Type == GameModeTypeSurvival) { return SurvivalModePreparePlayer(mode, player); }
}

void GameModeSpawnMob(GameMode mode)
{
	if (mode->Type == GameModeTypeSurvival) { return SurvivalModeSpawnMob(mode); }
}

void GameModePrepareLevel(GameMode mode, Level level)
{
	if (mode->Type == GameModeTypeSurvival) { return SurvivalModePrepareLevel(mode, level); }
}

bool GameModeIsSurvival(GameMode mode)
{
	if (mode->Type == GameModeTypeCreative) { return CreativeModeIsSurvival(mode); }
	return true;
}

void GameModeApplyPlayer(GameMode mode, Player player)
{
	if (mode->Type == GameModeTypeCreative) { return CreativeModeApplyPlayer(mode, player); }
}

void GameModeDestroy(GameMode mode)
{
	if (mode->Type == GameModeTypeSurvival) { SurvivalModeDestroy(mode); }
	free(mode);
}
