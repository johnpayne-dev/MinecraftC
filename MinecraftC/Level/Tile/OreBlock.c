#include "OreBlock.h"

OreBlock OreBlockCreate(BlockType type, int textureID)
{
	return BlockCreate(type, textureID);
}

BlockType OreBlockGetDrop(OreBlock block)
{
	return block->Type == BlockTypeCoalOre ? BlockTypeSlab : (block->Type == BlockTypeGoldOre ? BlockTypeGold : (block->Type == BlockTypeIronOre ? BlockTypeIron : block->Type));
}

int OreBlockGetDropCount(OreBlock block)
{
	return RandomIntegerRange(1, 3);
}
