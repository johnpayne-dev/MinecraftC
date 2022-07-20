#pragma once
#include "LoadLevelScreen.h"

typedef LoadLevelScreen SaveLevelScreen;

void SaveLevelScreenCreate(SaveLevelScreen * screen, GUIScreen * parent);
void SaveLevelScreenSetLevels(SaveLevelScreen * screen, char * strings[5]);
void SaveLevelScreenOnOpen(SaveLevelScreen * screen);
void SaveLevelScreenOpenLevel(SaveLevelScreen * screen, int level);
void SaveLevelScreenOpenLevelFromFile(SaveLevelScreen * screen, char * file);
