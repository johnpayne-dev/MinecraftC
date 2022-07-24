#pragma once
#include "GUIScreen.h"

typedef GUIScreen LevelNameScreen;

typedef struct LevelNameScreenData LevelNameScreenData;

void LevelNameScreenCreate(LevelNameScreen * screen, GUIScreen * parent, char * name, int id);
void LevelNameScreenOnOpen(LevelNameScreen * screen);
void LevelNameScreenOnClose(LevelNameScreen * screen);
void LevelNameScreenTick(LevelNameScreen * screen);
void LevelNameScreenRender(LevelNameScreen * screen, int mx, int my);
void LevelNameScreenOnKeyPressed(LevelNameScreen * screen, char eventChar, int eventKey);
void LevelNameScreenOnButtonClicked(LevelNameScreen * screen, Button * button);
void LevelNameScreenDestroy(LevelNameScreen * screen);
