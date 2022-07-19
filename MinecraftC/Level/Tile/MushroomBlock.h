#pragma once
#include "FlowerBlock.h"

typedef FlowerBlock MushroomBlock;

void MushroomBlockCreate(MushroomBlock * block, BlockType type, int textureID, TileSound sound, float particleGravity);
void MushroomBlockUpdate(MushroomBlock * block, struct Level * level, int x, int y, int z, RandomGenerator * random);
