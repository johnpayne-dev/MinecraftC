#pragma once
#include "GUIScreen.h"
#include "../GameSettings.h"

typedef GUIScreen OptionsScreen;

typedef struct OptionsScreenData {
	GUIScreen parent;
	char * title;
	GameSettings settings;
} * OptionsScreenData;

OptionsScreen OptionsScreenCreate(GUIScreen parent, GameSettings settings);
void OptionsScreenOnOpen(OptionsScreen screen);
void OptionsScreenOnButtonClicked(OptionsScreen screen, Button button);
void OptionsScreenRender(OptionsScreen screen, int mx, int my);
void OptionsScreenDestroy(OptionsScreen screen);
