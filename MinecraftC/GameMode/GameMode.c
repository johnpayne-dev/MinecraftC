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
	if (mode->Type == GameModeTypeCreative) { CreativeModeOpenInventory(mode); return; }
}

void GameModeHitBlock(GameMode mode, int x, int y, int z)
{
	if (mode->Type == GameModeTypeSurvival) { SurvivalModeHitBlock(mode, x, y, z); return; }
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
	if (mode->Type == GameModeTypeSurvival) { SurvivalModeHitBlockSide(mode, x, y, z, side); return; }
}

void GameModeResetHits(GameMode mode)
{
	if (mode->Type == GameModeTypeSurvival) { SurvivalModeResetHits(mode); return; }
}

void GameModeApplyCracks(GameMode mode, float cracks)
{
	if (mode->Type == GameModeTypeSurvival) { SurvivalModeApplyCracks(mode, cracks); return; }
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
	if (mode->Type == GameModeTypeSurvival) { SurvivalModePreparePlayer(mode, player); return; }
}

void GameModeSpawnMob(GameMode mode)
{
	if (mode->Type == GameModeTypeSurvival) { SurvivalModeSpawnMob(mode); return; }
}

void GameModePrepareLevel(GameMode mode, Level level)
{
	if (mode->Type == GameModeTypeSurvival) { SurvivalModePrepareLevel(mode, level); return; }
}

bool GameModeIsSurvival(GameMode mode)
{
	if (mode->Type == GameModeTypeCreative) { return CreativeModeIsSurvival(mode); }
	return true;
}

void GameModeApplyPlayer(GameMode mode, Player player)
{
	if (mode->Type == GameModeTypeCreative) { CreativeModeApplyPlayer(mode, player); return; }
}

void GameModeDestroy(GameMode mode)
{
	if (mode->Type == GameModeTypeSurvival) { SurvivalModeDestroy(mode); }
	free(mode);
}
