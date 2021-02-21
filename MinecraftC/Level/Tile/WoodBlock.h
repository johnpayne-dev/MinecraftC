#pragma once
#include "Block.h"

typedef Block WoodBlock;

WoodBlock WoodBlockCreate(void);
int WoodBlockGetDropCount(WoodBlock block);
BlockType WoodBlockGetDrop(WoodBlock block);
int WoodBlockGetTextureID(WoodBlock block, int face);
