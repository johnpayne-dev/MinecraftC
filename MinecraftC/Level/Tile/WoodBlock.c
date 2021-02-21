#include "WoodBlock.h"

WoodBlock WoodBlockCreate()
{
	return BlockCreate(BlockTypeLog, 20);
}

int WoodBlockGetDropCount(WoodBlock block)
{
	return RandomIntegerRange(0, 2) + 3;
}

BlockType WoodBlockGetDrop(WoodBlock block)
{
	return BlockTypeWood;
}

int WoodBlockGetTextureID(WoodBlock block, int face)
{
	return face == 1 ? 21 : (face == 0 ? 21 : 20);
}
