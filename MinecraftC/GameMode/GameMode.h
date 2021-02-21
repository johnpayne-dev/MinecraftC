#pragma once
#include <stdbool.h>
#include "../Level/Level.h"
#include "../Player/Player.h"

typedef enum GameModeType
{
	GameModeTypeNone,
	GameModeTypeCreative,
	GameModeTypeSurvival,
} GameModeType;

typedef struct GameMode
{
	struct Minecraft * Minecraft;
	bool InstantBreak;
	GameModeType Type;
	void * TypeData;
} * GameMode;

GameMode GameModeCreate(struct Minecraft * minecraft);
void GameModeApply(GameMode mode, Level level);
void GameModeOpenInventory(GameMode mode);
void GameModeHitBlock(GameMode mode, int x, int y, int z);
bool GameModeCanPlace(GameMode mode, BlockType blockType);
void GameModeBreakBlock(GameMode mode, int x, int y, int z);
void GameModeHitBlockSide(GameMode mode, int x, int y, int z, int side);
void GameModeResetHits(GameMode mode);
void GameModeApplyCracks(GameMode mode, float cracks);
float GameModeGetReachDistance(GameMode mode);
bool GameModeUseItem(GameMode mode, Player player, BlockType blockType);
void GameModePreparePlayer(GameMode mode, Player player);
void GameModeSpawnMob(GameMode mode);
void GameModePrepareLevel(GameMode mode, Level level);
bool GameModeIsSurvival(GameMode mode);
void GameModeApplyPlayer(GameMode mode, Player player);
void GameModeDestroy(GameMode gameMode);
