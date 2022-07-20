#pragma once
#include "GUIScreen.h"
#include "../GameSettings.h"

typedef GUIScreen OptionsScreen;

typedef struct OptionsScreenData OptionsScreenData;

void OptionsScreenCreate(OptionsScreen * screen, GUIScreen * parent, GameSettings * settings);
void OptionsScreenOnOpen(OptionsScreen * screen);
void OptionsScreenOnButtonClicked(OptionsScreen * screen, Button * button);
void OptionsScreenRender(OptionsScreen * screen, int mx, int my);
