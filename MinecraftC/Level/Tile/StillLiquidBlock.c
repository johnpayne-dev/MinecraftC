#include "StillLiquidBlock.h"
#include "../Level.h"

StillLiquidBlock StillLiquidBlockCreate(BlockType blockType, LiquidType liquidType) {
	LiquidBlock block = LiquidBlockCreate(blockType, liquidType);
	LiquidBlockData liquid = block->typeData;
	liquid->movingID = blockType - 1;
	liquid->stillID = blockType;
	BlockSetPhysics(block, false);
	return block;
}

void StillLiquidBlockUpdate(StillLiquidBlock block, Level level, int x, int y, int z, RandomGenerator random) {
	return;
}

void StillLiquidBlockOnNeighborChanged(StillLiquidBlock block, Level level, int x, int y, int z, BlockType tile) {
	LiquidBlockData liquid = block->typeData;
	bool update = false;
	if (LevelGetTile(level, x - 1, y, z) == BlockTypeNone) { update = true; }
	if (LevelGetTile(level, x + 1, y, z) == BlockTypeNone) { update = true; }
	if (LevelGetTile(level, x, y - 1, z) == BlockTypeNone) { update = true; }
	if (LevelGetTile(level, x, y, z - 1) == BlockTypeNone) { update = true; }
	if (LevelGetTile(level, x, y, z + 1) == BlockTypeNone) { update = true; }
	if (tile != BlockTypeNone) {
		LiquidType liquidType = BlockGetLiquidType(Blocks.table[tile]);
		if ((liquid->type == LiquidTypeWater && liquidType == LiquidTypeLava) || (liquidType == LiquidTypeWater && liquid->type == LiquidTypeLava)) {
			LevelSetTile(level, x, y, z, BlockTypeStone);
			return;
		}
	}
	if (update) {
		LevelSetTileNoUpdate(level, x, y, z, liquid->movingID);
		LevelAddToNextTick(level, x, y, z, liquid->movingID);
	}
}
