#pragma once
#include "FlowerBlock.h"

typedef FlowerBlock SaplingBlock;

SaplingBlock SaplingBlockCreate(void);
void SaplingBlockUpdate(SaplingBlock block, struct Level * level, int x, int y, int z, RandomGenerator random);
