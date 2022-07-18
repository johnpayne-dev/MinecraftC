#pragma once
#include <SDL2/SDL_rwops.h>
#include "../ProgressBarDisplay.h"
#include "Level.h"

typedef struct LevelIO {
	ProgressBarDisplay progressBar;
} * LevelIO;

LevelIO LevelIOCreate(ProgressBarDisplay progress);
bool LevelIOSave(LevelIO levelIO, Level level, SDL_RWops * output);
Level LevelIOLoad(LevelIO levelIO, SDL_RWops * input);
void LevelIODestroy(LevelIO levelIO);
