#include "LeavesBlock.h"
#include "../Level.h"

void LeavesBlockCreate(LeavesBlock * block, TileSound sound, float particleGravity) {
	BlockCreate(block, BlockTypeLeaves, 22, sound, particleGravity);
}

bool LeavesBlockCanRenderSide(LeavesBlock * block, Level * level, int x, int y, int z, int side) {
	return !LevelIsSolidTile(level, x, y, z);
}

bool LeavesBlockIsSolid(LeavesBlock * block) {
	return false;
}

bool LeavesBlockIsOpaque(LeavesBlock * block) {
	return false;
}
