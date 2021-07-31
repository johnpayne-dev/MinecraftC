#pragma once
#include "Block.h"

typedef Block SlabBlock;

typedef struct SlabBlockData
{
	bool DoubleSlab;
} * SlabBlockData;

SlabBlock SlabBlockCreate(BlockType type, bool doubleSlab);
int SlabBlockGetTextureID(SlabBlock block, int face);
bool SlabBlockIsSolid(SlabBlock block);
void SlabBlockOnNeighborChanged(SlabBlock block, struct Level * level, int x, int y, int z, int side);
void SlabBlockOnAdded(SlabBlock block, struct Level * level, int x, int y, int z);
bool SlabBlockIsCube(SlabBlock block);
bool SlabBlockCanRenderSide(SlabBlock block, struct Level * level, int x, int y, int z, int side);
void SlabBlockDestroy(SlabBlock block);
