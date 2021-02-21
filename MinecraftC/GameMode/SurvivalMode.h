#pragma once
#include "GameMode.h"
#include "../Level/MobSpawner.h"

typedef GameMode SurvivalMode;

typedef struct SurvivalModeData
{
	int3 Hit;
	int Hits;
	int Hardness;
	int HitDelay;
	MobSpawner MobSpawner;
} * SurvivalModeData;

SurvivalMode SurvivalModeCreate(struct Minecraft * minecraft);
void SurvivalModePreparePlayer(SurvivalMode mode, Player player);
void SurvivalModeBreakBlock(SurvivalMode mode, int x, int y, int z);
bool SurvivalModeCanPlace(SurvivalMode mode, BlockType blockType);
void SurvivalModeHitBlock(SurvivalMode mode, int x, int y, int z);
void SurvivalModeResetHits(SurvivalMode mode);
void SurvivalModeHitBlockSide(SurvivalMode mode, int x, int y, int z, int side);
void SurvivalModeApplyCracks(SurvivalMode mode, float cracks);
float SurvivalModeGetReachDistance(SurvivalMode mode);
bool SurvivalModeUseItem(SurvivalMode mode, Player player, BlockType blockType);
void SurvivalModeApply(SurvivalMode mode, Level level);
void SurvivalModeSpawnMob(SurvivalMode mode);
void SurvivalModePrepareLevel(SurvivalMode mode, Level level);
void SurvivalModeDestroy(SurvivalMode mode);
