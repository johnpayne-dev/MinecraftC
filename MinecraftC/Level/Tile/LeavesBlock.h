#pragma once
#include "Block.h"

typedef Block LeavesBlock;

void LeavesBlockCreate(LeavesBlock * block, TileSound sound, float particleGravity);
bool LeavesBlockCanRenderSide(LeavesBlock * block, struct Level * level, int x, int y, int z, int side);
bool LeavesBlockIsSolid(LeavesBlock * block);
bool LeavesBlockIsOpaque(LeavesBlock * block);
