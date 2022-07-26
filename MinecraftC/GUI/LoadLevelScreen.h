#pragma once
#include <SDL.h>
#include "GUIScreen.h"

typedef GUIScreen LoadLevelScreen;

typedef struct LoadLevelScreenData LoadLevelScreenData;

void LoadLevelScreenCreate(LoadLevelScreen * screen, GUIScreen * parent);
void LoadLevelScreenOnOpen(LoadLevelScreen * screen);
void LoadLevelScreenOnButtonClicked(LoadLevelScreen * screen, Button * button);
void LoadLevelScreenOpenLevel(LoadLevelScreen * screen, int level);
void LoadLevelScreenOpenLevelFromFile(LoadLevelScreen * screen, char * file);
void LoadLevelScreenRender(LoadLevelScreen * screen, int mx, int my);
