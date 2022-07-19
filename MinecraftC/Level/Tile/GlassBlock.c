#include "GlassBlock.h"
#include "../Level.h"

GlassBlock GlassBlockCreate(void) {
	return BlockCreate(BlockTypeGlass, 49);
}

bool GlassBlockCanRenderSide(GlassBlock block, Level * level, int x, int y, int z, int side) {
	BlockType tile = LevelGetTile(level, x, y, z);
	return tile == block->type ? false : !LevelIsSolidTile(level, x, y, z);
}

bool GlassBlockIsOpaque(GlassBlock block) {
	return false;
}

bool GlassBlockIsSolid(GlassBlock block) {
	return false;
}
