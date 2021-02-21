#pragma once
#include "Block.h"

typedef Block SandBlock;

SandBlock SandBlockCreate(BlockType type, int texture);
void SandBlockOnNeighborChanged(SandBlock block, struct Level * level, int x, int y, int z, BlockType tile);
void SandBlockOnPlaced(SandBlock block, struct Level * level, int x, int y, int z);
