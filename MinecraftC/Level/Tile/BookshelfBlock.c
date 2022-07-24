#include "BookshelfBlock.h"

void BookshelfBlockCreate(BookshelfBlock * block, TileSound sound, float particleGravity) {
	BlockCreate(block, BlockTypeBookshelf, 35, sound, particleGravity);
}

int BookshelfBlockGetTextureID(BookshelfBlock * block, int face) {
	return face <= 1 ? 4 : block->textureID;
}
