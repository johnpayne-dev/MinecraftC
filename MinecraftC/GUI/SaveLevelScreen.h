#pragma once
#include "LoadLevelScreen.h"

typedef LoadLevelScreen SaveLevelScreen;

SaveLevelScreen SaveLevelScreenCreate(GUIScreen parent);
void SaveLevelScreenSetLevels(SaveLevelScreen screen, char * strings[5]);
void SaveLevelScreenOnOpen(SaveLevelScreen screen);
void SaveLevelScreenOpenLevel(SaveLevelScreen screen, int level);
void SaveLevelScreenOpenLevelFromFile(SaveLevelScreen screen, char * file);
void SaveLevelScreenRender(SaveLevelScreen screen, int2 mousePos);
