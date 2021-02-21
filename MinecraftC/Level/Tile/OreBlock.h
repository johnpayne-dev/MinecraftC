#pragma once
#include "Block.h"

typedef Block OreBlock;

OreBlock OreBlockCreate(BlockType type, int textureID);
BlockType OreBlockGetDrop(OreBlock block);
int OreBlockGetDropCount(OreBlock block);
