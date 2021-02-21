#pragma once
#include "FlowerBlock.h"

typedef FlowerBlock MushroomBlock;

MushroomBlock MushroomBlockCreate(BlockType type, int textureID);
void MushroomBlockUpdate(MushroomBlock block, struct Level * level, int x, int y, int z, RandomGenerator random);
