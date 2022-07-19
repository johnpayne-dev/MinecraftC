#include "LeavesBlock.h"
#include "../Level.h"

LeavesBlock LeavesBlockCreate() {
	return BlockCreate(BlockTypeLeaves, 22);
}

bool LeavesBlockCanRenderSide(LeavesBlock block, Level * level, int x, int y, int z, int side) {
	return !LevelIsSolidTile(level, x, y, z);
}

bool LeavesBlockIsSolid(LeavesBlock block) {
	return false;
}

bool LeavesBlockIsOpaque(LeavesBlock block) {
	return false;
}

int LeavesBlockGetDropCount(LeavesBlock block) {
	return RandomIntegerRange(0, 9) == 0 ? 1 : 0;
}

BlockType LeavesBlockGetDrop(LeavesBlock block) {
	return BlockTypeSapling;
}

