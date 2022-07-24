#include "StillLiquidBlock.h"
#include "../Level.h"

void StillLiquidBlockCreate(StillLiquidBlock * block, BlockType blockType, LiquidType liquidType, TileSound sound, float particleGravity) {
	LiquidBlockCreate(block, blockType, liquidType, sound, particleGravity);
	block->liquid.movingID = blockType - 1;
	block->liquid.stillID = blockType;
	BlockSetPhysics(block, false);
}

void StillLiquidBlockUpdate(StillLiquidBlock * block, Level * level, int x, int y, int z, RandomGenerator * random) {
	return;
}

void StillLiquidBlockOnNeighborChanged(StillLiquidBlock * block, Level * level, int x, int y, int z, BlockType tile) {
	bool update = false;
	if (LevelGetTile(level, x - 1, y, z) == BlockTypeNone) { update = true; }
	if (LevelGetTile(level, x + 1, y, z) == BlockTypeNone) { update = true; }
	if (LevelGetTile(level, x, y - 1, z) == BlockTypeNone) { update = true; }
	if (LevelGetTile(level, x, y, z - 1) == BlockTypeNone) { update = true; }
	if (LevelGetTile(level, x, y, z + 1) == BlockTypeNone) { update = true; }
	if (tile != BlockTypeNone) {
		LiquidType liquidType = BlockGetLiquidType(&Blocks.table[tile]);
		if ((block->liquid.type == LiquidTypeWater && liquidType == LiquidTypeLava) || (liquidType == LiquidTypeWater && block->liquid.type == LiquidTypeLava)) {
			LevelSetTile(level, x, y, z, BlockTypeStone);
			return;
		}
	}
	if (update) {
		LevelSetTileNoUpdate(level, x, y, z, block->liquid.movingID);
		LevelAddToNextTick(level, x, y, z, block->liquid.movingID);
	}
}
