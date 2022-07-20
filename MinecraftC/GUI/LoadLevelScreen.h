#pragma once
#include <SDL2/SDL.h>
#include "GUIScreen.h"

typedef GUIScreen LoadLevelScreen;

typedef struct LoadLevelScreenData LoadLevelScreenData;

void LoadLevelScreenCreate(LoadLevelScreen * screen, GUIScreen * parent);
void LoadLevelScreenRun(LoadLevelScreen * screen);
void LoadLevelScreenSetLevels(LoadLevelScreen * screen, char * strings[5]);
void LoadLevelScreenOnOpen(LoadLevelScreen * screen);
void LoadLevelScreenOnButtonClicked(LoadLevelScreen * screen, Button * button);
void LoadLevelScreenOpenLevel(LoadLevelScreen * screen, int level);
void LoadLevelScreenOpenLevelFromFile(LoadLevelScreen * screen, char * file);
void LoadLevelScreenOnClose(LoadLevelScreen * screen);
void LoadLevelScreenTick(LoadLevelScreen * screen);
void LoadLevelScreenRender(LoadLevelScreen * screen, int mx, int my);
