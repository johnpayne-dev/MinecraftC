#pragma once
#include "Block.h"

typedef Block LiquidBlock;

void LiquidBlockCreate(LiquidBlock * block, BlockType blockType, LiquidType liquidType, TileSound sound, float particleGravity);
bool LiquidBlockIsCube(LiquidBlock * block);
void LiquidBlockOnPlaced(LiquidBlock * block, struct Level * level, int x, int y, int z);
void LiquidBlockUpdate(LiquidBlock * block, struct Level * level, int x, int y, int z, RandomGenerator * random);
float LiquidBlockGetBrightness(LiquidBlock * block, struct Level * level, int x, int y, int z);
bool LiquidBlockCanRenderSide(LiquidBlock * block, struct Level * level, int x, int y, int z, int side);
void LiquidBlockRenderInside(LiquidBlock * block, int x, int y, int z, int side);
AABB LiquidBlockGetSelectionAABB(LiquidBlock * block, int x, int y, int z);
bool LiquidBlockIsOpaque(LiquidBlock * block);
bool LiquidBlockIsSolid(LiquidBlock * block);
LiquidType LiquidBlockGetLiquidType(LiquidBlock * block);
void LiquidBlockOnNeighborChanged(LiquidBlock * block, struct Level * level, int x, int y, int z, BlockType tile);
int LiquidBlockGetTickDelay(LiquidBlock * block);
int LiquidBlockGetRenderPass(LiquidBlock * block);
