#include "SlabBlock.h"
#include "../Level.h"

SlabBlock SlabBlockCreate(BlockType type, bool doubleSlab) {
	Block block = BlockCreate(type, 6);
	SlabBlockData slab = malloc(sizeof(struct SlabBlockData));
	block->typeData = slab;
	*slab = (struct SlabBlockData){ .DoubleSlab = doubleSlab, };
	if (!doubleSlab) { BlockSetBounds(block, 0, 0, 0, 1, 0.5, 1); }
	return block;
}

int SlabBlockGetTextureID(SlabBlock block, int face) {
	return face <= 1 ? 6 : 5;
}

bool SlabBlockIsSolid(SlabBlock block) {
	return block->type == BlockTypeDoubleSlab;
}

void SlabBlockOnNeighborChanged(SlabBlock block, Level * level, int x, int y, int z, int side) {
	if (block->type == BlockTypeSlab) {
	}
}

void SlabBlockOnAdded(SlabBlock block, Level * level, int x, int y, int z) {
	if (LevelGetTile(level, x, y - 1, z) == BlockTypeSlab) {
		LevelSetTile(level, x, y, z, BlockTypeNone);
		LevelSetTile(level, x, y - 1, z, BlockTypeDoubleSlab);
	}
}

BlockType SlabBlockGetDrop(SlabBlock block) {
	return BlockTypeSlab;
}

bool SlabBlockIsCube(SlabBlock block) {
	return block->type == BlockTypeDoubleSlab;
}

bool SlabBlockCanRenderSide(SlabBlock block, Level * level, int x, int y, int z, int side) {
	return side == 1 ? true : (LevelIsSolidTile(level, x, y, z) ? false : (side == 0 ? true : LevelGetTile(level, x, y, z) != block->type));
}

void SlabBlockDestroy(SlabBlock block) {
	free(block->typeData);
}
