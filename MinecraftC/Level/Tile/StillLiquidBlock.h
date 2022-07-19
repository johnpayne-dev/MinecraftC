#pragma once
#include "LiquidBlock.h"

typedef LiquidBlock StillLiquidBlock;

StillLiquidBlock StillLiquidBlockCreate(BlockType blockType, LiquidType liquidType);
void StillLiquidBlockUpdate(StillLiquidBlock block, struct Level * level, int x, int y, int z, RandomGenerator * random);
void StillLiquidBlockOnNeighborChanged(StillLiquidBlock block, struct Level * level, int x, int y, int z, BlockType tile);
