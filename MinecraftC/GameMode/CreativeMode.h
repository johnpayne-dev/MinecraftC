#pragma once
#include "GameMode.h"

typedef GameMode CreativeMode;

CreativeMode CreativeModeCreate(struct Minecraft * minecraft);
void CreativeModeOpenInventory(CreativeMode mode);
void CreativeModeApply(CreativeMode mode, Level level);
void CreativeModeApplyPlayer(CreativeMode mode, Player player);
bool CreativeModeIsSurvival(CreativeMode mode);
