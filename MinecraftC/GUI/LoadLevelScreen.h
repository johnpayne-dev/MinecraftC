#pragma once
#include <SDL2/SDL.h>
#include "GUIScreen.h"

typedef GUIScreen LoadLevelScreen;

typedef struct LoadLevelScreenData {
	GUIScreen parent;
	bool finished;
	bool loaded;
	char * levels[5];
	char * status;
	char * title;
	bool frozen;
	bool saving;
	char * selectedFile;
} * LoadLevelScreenData;

LoadLevelScreen LoadLevelScreenCreate(GUIScreen parent);
void LoadLevelScreenRun(LoadLevelScreen screen);
void LoadLevelScreenSetLevels(LoadLevelScreen screen, char * strings[5]);
void LoadLevelScreenOnOpen(LoadLevelScreen screen);
void LoadLevelScreenOnButtonClicked(LoadLevelScreen screen, Button button);
void LoadLevelScreenOpenLevel(LoadLevelScreen screen, int level);
void LoadLevelScreenOpenLevelFromFile(LoadLevelScreen screen, char * file);
void LoadLevelScreenOnClose(LoadLevelScreen screen);
void LoadLevelScreenTick(LoadLevelScreen screen);
void LoadLevelScreenRender(LoadLevelScreen screen, int2 mousePos);
void LoadLevelScreenDestroy(LoadLevelScreen screen);
