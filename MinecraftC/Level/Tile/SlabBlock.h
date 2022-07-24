#pragma once
#include "Block.h"

typedef Block SlabBlock;

void SlabBlockCreate(SlabBlock * block, BlockType type, bool doubleSlab, TileSound sound, float particleGravity);
int SlabBlockGetTextureID(SlabBlock * block, int face);
bool SlabBlockIsSolid(SlabBlock * block);
void SlabBlockOnNeighborChanged(SlabBlock * block, struct Level * level, int x, int y, int z, int side);
void SlabBlockOnAdded(SlabBlock * block, struct Level * level, int x, int y, int z);
bool SlabBlockIsCube(SlabBlock * block);
bool SlabBlockCanRenderSide(SlabBlock * block, struct Level * level, int x, int y, int z, int side);
