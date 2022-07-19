#include "SaplingBlock.h"
#include "../Level.h"

void SaplingBlockCreate(SaplingBlock * block, TileSound sound, float particleGravity) {
	FlowerBlockCreate(block, BlockTypeSapling, 15, sound, particleGravity);
	BlockSetBounds(block, 0.1, 0.0, 0.1, 0.9, 0.8, 0.9);
}

void SaplingBlockUpdate(SaplingBlock * block, Level * level, int x, int y, int z, RandomGenerator * random) {
	BlockType tile = LevelGetTile(level, x, y - 1, z);
	if (LevelIsLit(level, x, y, z) && (tile == BlockTypeDirt || tile == BlockTypeGrass)) {
		if (RandomIntegerRange(0, 4) == 0) {
			LevelSetTileNoUpdate(level, x, y, z, BlockTypeNone);
			if (!LevelMaybeGrowTree(level, x, y, z)) { LevelSetTileNoUpdate(level, x, y, z, block->type); }
		}
	} else { LevelSetTile(level, x, y, z, BlockTypeNone); }
}
