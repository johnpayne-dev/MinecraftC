#include "LevelIO.h"

LevelIO LevelIOCreate(ProgressBarDisplay progress) {
	LevelIO levelIO = MemoryAllocate(sizeof(struct LevelIO));
	*levelIO = (struct LevelIO){ .progressBar = progress, };
	return levelIO;
}

bool LevelIOSave(LevelIO levelIO, Level level, SDL_RWops * output) {
	ProgressBarDisplaySetText(levelIO->progressBar, "Failed!");
	return false;
}

Level LevelIOLoad(LevelIO levelIO, SDL_RWops * input) {
	ProgressBarDisplaySetText(levelIO->progressBar, "Failed!");
	return NULL;
}

void LevelIODestroy(LevelIO levelIO) {
	MemoryFree(levelIO);
}
