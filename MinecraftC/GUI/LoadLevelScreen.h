#pragma once
#include <SDL2/SDL.h>
#include "GUIScreen.h"

typedef GUIScreen LoadLevelScreen;

typedef struct LoadLevelScreenData
{
	GUIScreen Parent;
	bool Finished;
	bool Loaded;
	char * Levels[5];
	char * Status;
	char * Title;
	bool Frozen;
	bool Saving;
	char * SelectedFile;
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
