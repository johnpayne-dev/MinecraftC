#include "SlabBlock.h"
#include "../Level.h"

void SlabBlockCreate(SlabBlock * block, BlockType type, bool doubleSlab, TileSound sound, float particleGravity) {
	BlockCreate(block, type, 6, sound, particleGravity);
	block->slab.doubleSlab = doubleSlab;
	if (!doubleSlab) { BlockSetBounds(block, 0, 0, 0, 1, 0.5, 1); }
}

int SlabBlockGetTextureID(SlabBlock * block, int face) {
	return face <= 1 ? 6 : 5;
}

bool SlabBlockIsSolid(SlabBlock * block) {
	return block->type == BlockTypeDoubleSlab;
}

void SlabBlockOnNeighborChanged(SlabBlock * block, Level * level, int x, int y, int z, int side) {
	if (block->type == BlockTypeSlab) {
	}
}

void SlabBlockOnAdded(SlabBlock * block, Level * level, int x, int y, int z) {
	if (LevelGetTile(level, x, y - 1, z) == BlockTypeSlab) {
		LevelSetTile(level, x, y, z, BlockTypeNone);
		LevelSetTile(level, x, y - 1, z, BlockTypeDoubleSlab);
	}
}

bool SlabBlockIsCube(SlabBlock * block) {
	return block->type == BlockTypeDoubleSlab;
}

bool SlabBlockCanRenderSide(SlabBlock * block, Level * level, int x, int y, int z, int side) {
	return side == 1 ? true : (LevelIsSolidTile(level, x, y, z) ? false : (side == 0 ? true : LevelGetTile(level, x, y, z) != block->type));
}
