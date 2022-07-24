#pragma once
#include "Block.h"

typedef Block SandBlock;

void SandBlockCreate(SandBlock * block, BlockType type, int texture, TileSound sound, float particleGravity);
void SandBlockOnNeighborChanged(SandBlock * block, struct Level * level, int x, int y, int z, BlockType tile);
void SandBlockOnPlaced(SandBlock * block, struct Level * level, int x, int y, int z);
