#include "StoneBlock.h"

StoneBlock StoneBlockCreate(int textureID) {
	return BlockCreate(BlockTypeStone, textureID);
}

BlockType StoneBlockGetDrop(StoneBlock block) {
	return BlockTypeCobbleStone;
}
