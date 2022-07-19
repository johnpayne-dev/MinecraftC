#include "MushroomBlock.h"
#include "../Level.h"

MushroomBlock MushroomBlockCreate(BlockType type, int textureID) {
	FlowerBlock block = FlowerBlockCreate(type, textureID);
	BlockSetBounds(block, 0.3, 0.0, 0.3, 0.7, 0.4, 0.7);
	return block;
}

void MushroomBlockUpdate(MushroomBlock block, Level * level, int x, int y, int z, RandomGenerator * random) {
	BlockType tile = LevelGetTile(level, x, y - 1, z);
	if (LevelIsLit(level, x, y, z) || (tile != BlockTypeStone && tile != BlockTypeGravel && tile != BlockTypeCobbleStone)) {
		LevelSetTile(level, x, y, z, BlockTypeNone);
	}
}
