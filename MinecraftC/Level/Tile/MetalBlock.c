#include "MetalBlock.h"

void MetalBlockCreate(MetalBlock * block, BlockType type, int textureID, TileSound sound, float particleGravity) {
	BlockCreate(block, type, textureID, sound, particleGravity);
}

int MetalBlockGetTextureID(MetalBlock * block, int face) {
	return face == 1 ? block->textureID - 16 : (face == 0 ? block->textureID + 16 : block->textureID);
}
