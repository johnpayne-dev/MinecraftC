#pragma once
#include "Block.h"

typedef Block WoodBlock;

WoodBlock WoodBlockCreate(void);
int WoodBlockGetTextureID(WoodBlock block, int face);
