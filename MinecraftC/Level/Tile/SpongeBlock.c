#include "SpongeBlock.h"
#include "../Level.h"

void SpongeBlockCreate(SpongeBlock * block, TileSound sound, float particleGravity) {
	BlockCreate(block, BlockTypeSponge, 48, sound, particleGravity);
}

void SpongeBlockOnAdded(SpongeBlock * block, Level * level, int x, int y, int z) {
	for (int i = x - 2; i <= x + 2; i++) {
		for (int j = y - 2; j <= y + 2; j++) {
			for (int k = z - 2; k <= z + 2; k++) {
				if (LevelIsWater(level, i, j, k)) { LevelSetTileNoNeighborChange(level, i, j, k, BlockTypeNone); }
			}
		}
	}
}

void SpongeBlockOnRemoved(SpongeBlock * block, Level * level, int x, int y, int z) {
	for (int i = x - 2; i <= x + 2; i++) {
		for (int j = y - 2; j <= y + 2; j++) {
			for (int k = z - 2; k <= z + 2; k++) {
				LevelUpdateNeighborsAt(level, i, j, k, LevelGetTile(level, i, j, k));
			}
		}
	}
}
