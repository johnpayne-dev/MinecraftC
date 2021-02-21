#include "MetalBlock.h"

MetalBlock MetalBlockCreate(BlockType type, int textureID)
{
	return BlockCreate(type, textureID);
}

int MetalBlockGetTextureID(MetalBlock block, int face)
{
	return face == 1 ? block->TextureID - 16 : (face == 0 ? block->TextureID + 16 : block->TextureID);
}
