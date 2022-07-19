#pragma once
#include "GUIScreen.h"
#include "../GameSettings.h"

typedef GUIScreen ControlsScreen;

typedef struct ControlsScreenData {
	GUIScreen parent;
	char * title;
	GameSettings * settings;
	int selected;
} * ControlsScreenData;

ControlsScreen ControlsScreenCreate(GUIScreen parent, GameSettings * settings);
void ControlsScreenOnOpen(ControlsScreen screen);
void ControlsScreenRender(ControlsScreen screen, int mx, int my);
void ControlsScreenOnKeyPressed(ControlsScreen screen, char eventChar, int eventKey);
void ControlsScreenOnButtonClicked(ControlsScreen screen, Button button);
void ControlsScreenDestroy(ControlsScreen screen);
