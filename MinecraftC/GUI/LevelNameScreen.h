#pragma once
#include "GUIScreen.h"

typedef GUIScreen LevelNameScreen;

typedef struct LevelNameScreenData {
	GUIScreen parent;
	char * title;
	int id;
	String name;
	int counter;
} * LevelNameScreenData;

LevelNameScreen LevelNameScreenCreate(GUIScreen parent, char * name, int id);
void LevelNameScreenOnOpen(LevelNameScreen screen);
void LevelNameScreenOnClose(LevelNameScreen screen);
void LevelNameScreenTick(LevelNameScreen screen);
void LevelNameScreenRender(LevelNameScreen screen, int mx, int my);
void LevelNameScreenOnKeyPressed(LevelNameScreen screen, char eventChar, int eventKey);
void LevelNameScreenOnButtonClicked(LevelNameScreen screen, Button button);
void LevelNameScreenDestroy(LevelNameScreen screen);
