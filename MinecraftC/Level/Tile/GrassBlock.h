#pragma once
#include "Block.h"

typedef Block GrassBlock;

GrassBlock GrassBlockCreate(void);
int GrassBlockGetTextureID(GrassBlock block, int side);
void GrassBlockUpdate(GrassBlock block, struct Level * level, int x, int y, int z, RandomGenerator random);
