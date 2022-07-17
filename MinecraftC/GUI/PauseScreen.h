#pragma once
#include "GUIScreen.h"

typedef GUIScreen PauseScreen;

PauseScreen PauseScreenCreate(void);
void PauseScreenOnOpen(PauseScreen screen);
void PauseScreenOnButtonClicked(PauseScreen screen, Button button);
void PauseScreenRender(PauseScreen screen, int mx, int my);
