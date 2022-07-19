#include "LogBlock.h"

void LogBlockCreate(LogBlock * block, TileSound sound, float particleGravity) {
	BlockCreate(block, BlockTypeLog, 20, sound, particleGravity);
}

int LogBlockGetTextureID(LogBlock * block, int face) {
	return face == 1 ? 21 : (face == 0 ? 21 : 20);
}
