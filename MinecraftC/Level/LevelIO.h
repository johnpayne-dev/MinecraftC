#pragma once
#include <SDL2/SDL_rwops.h>
#include "../ProgressBarDisplay.h"
#include "Level.h"

typedef struct LevelIO
{
	ProgressBarDisplay ProgressBar;
} * LevelIO;

LevelIO LevelIOCreate(ProgressBarDisplay progress);
bool LevelIOSave(LevelIO levelIO, Level level, SDL_RWops * output);
Level LevelIOLoad(LevelIO levelIO, SDL_RWops * input);
bool LevelIOSaveOnline(LevelIO levelIO, Level level, char * host, char * userName, char * sessionID, char * levelName, int id);
Level LevelIOLoadOnline(LevelIO levelIO, char * url, char * user, int id);
void LevelIODestroy(LevelIO levelIO);
