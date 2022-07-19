#pragma once
#include "LiquidBlock.h"

typedef LiquidBlock StillLiquidBlock;

void StillLiquidBlockCreate(StillLiquidBlock * block, BlockType blockType, LiquidType liquidType, TileSound sound, float particleGravity);
void StillLiquidBlockUpdate(StillLiquidBlock * block, struct Level * level, int x, int y, int z, RandomGenerator * random);
void StillLiquidBlockOnNeighborChanged(StillLiquidBlock * block, struct Level * level, int x, int y, int z, BlockType tile);
