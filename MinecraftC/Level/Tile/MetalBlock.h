#pragma once
#include "Block.h"

typedef Block MetalBlock;

void MetalBlockCreate(MetalBlock * block, BlockType type, int textureID, TileSound sound, float particleGravity);
int MetalBlockGetTextureID(MetalBlock * block, int face);
