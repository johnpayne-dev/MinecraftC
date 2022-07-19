#include "GrassBlock.h"
#include "../Level.h"

void GrassBlockCreate(GrassBlock * block, TileSound sound, float particleGravity) {
	BlockCreate(block, BlockTypeGrass, 3, sound, particleGravity);
	BlockSetPhysics(block, true);
}

int GrassBlockGetTextureID(GrassBlock * block, int side) {
	return side == 1 ? 0 : (side == 0 ? 2 : 3);
}

void GrassBlockUpdate(GrassBlock * block, Level * level, int x, int y, int z, RandomGenerator * random) {
	if (RandomGeneratorIntegerRange(random, 0, 3) == 0) {
		if (!LevelIsLit(level, x, y, z)) {
			LevelSetTile(level, x, y, z, BlockTypeDirt);
		} else {
			for (int i = 0; i < 4; i++) {
				int vx = x + (int)RandomGeneratorIntegerRange(random, 0, 2) - 1;
				int vy = y + (int)RandomGeneratorIntegerRange(random, 0, 4) - 3;
				int vz = z + (int)RandomGeneratorIntegerRange(random, 0, 2) - 1;
				if (LevelGetTile(level, vx, vy, vz) == BlockTypeDirt && LevelIsLit(level, vx, vy, vz)) { LevelSetTile(level, vx, vy, vz, BlockTypeGrass); }
			}
		}
	}
}
