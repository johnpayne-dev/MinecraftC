#include "MetalBlock.h"

MetalBlock MetalBlockCreate(BlockType type, int textureID) {
	return BlockCreate(type, textureID);
}

int MetalBlockGetTextureID(MetalBlock block, int face) {
	return face == 1 ? block->textureID - 16 : (face == 0 ? block->textureID + 16 : block->textureID);
}
