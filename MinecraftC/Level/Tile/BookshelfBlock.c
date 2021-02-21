#include "BookshelfBlock.h"

BookshelfBlock BookshelfBlockCreate()
{
	return BlockCreate(BlockTypeBookshelf, 35);
}

int BookshelfBlockGetTextureID(BookshelfBlock block, int face)
{
	return face <= 1 ? 4 : block->TextureID;
}

int BookshelfBlockGetDropCount(BookshelfBlock block)
{
	return 0;
}
