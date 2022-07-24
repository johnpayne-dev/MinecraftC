#include "MushroomBlock.h"
#include "../Level.h"

void MushroomBlockCreate(MushroomBlock * block, BlockType type, int textureID, TileSound sound, float particleGravity) {
	FlowerBlockCreate(block, type, textureID, sound, particleGravity);
	BlockSetBounds(block, 0.3, 0.0, 0.3, 0.7, 0.4, 0.7);
}

void MushroomBlockUpdate(MushroomBlock * block, Level * level, int x, int y, int z, RandomGenerator * random) {
	BlockType tile = LevelGetTile(level, x, y - 1, z);
	if (LevelIsLit(level, x, y, z) || (tile != BlockTypeStone && tile != BlockTypeGravel && tile != BlockTypeCobbleStone)) {
		LevelSetTile(level, x, y, z, BlockTypeNone);
	}
}
