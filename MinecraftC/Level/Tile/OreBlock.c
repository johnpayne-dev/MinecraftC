#include "OreBlock.h"

OreBlock OreBlockCreate(BlockType type, int textureID) {
	return BlockCreate(type, textureID);
}

BlockType OreBlockGetDrop(OreBlock block) {
	return block->type == BlockTypeCoalOre ? BlockTypeSlab : (block->type == BlockTypeGoldOre ? BlockTypeGold : (block->type == BlockTypeIronOre ? BlockTypeIron : block->type));
}

int OreBlockGetDropCount(OreBlock block) {
	return RandomIntegerRange(1, 3);
}
