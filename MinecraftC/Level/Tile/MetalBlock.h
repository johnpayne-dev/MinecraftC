#pragma once
#include "Block.h"

typedef Block MetalBlock;

MetalBlock MetalBlockCreate(BlockType type, int textureID);
int MetalBlockGetTextureID(MetalBlock block, int face);
