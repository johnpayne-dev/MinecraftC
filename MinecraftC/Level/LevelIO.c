#include "LevelIO.h"

void LevelIOCreate(LevelIO * levelIO, ProgressBarDisplay * progress) {
	*levelIO = (LevelIO){ .progressBar = progress, };
}

bool LevelIOSave(LevelIO * levelIO, Level * level, SDL_RWops * output) {
	ProgressBarDisplaySetText(levelIO->progressBar, "Failed!");
	return false;
}

Level * LevelIOLoad(LevelIO * levelIO, SDL_RWops * input) {
	ProgressBarDisplaySetText(levelIO->progressBar, "Failed!");
	return NULL;
}
