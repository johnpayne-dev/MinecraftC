#pragma once
#include "FlowerBlock.h"

typedef FlowerBlock SaplingBlock;

void SaplingBlockCreate(SaplingBlock * block, TileSound sound, float particleGravity);
void SaplingBlockUpdate(SaplingBlock * block, struct Level * level, int x, int y, int z, RandomGenerator * random);
