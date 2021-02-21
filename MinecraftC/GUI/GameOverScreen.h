#pragma once
#include "GUIScreen.h"

typedef GUIScreen GameOverScreen;

GameOverScreen GameOverScreenCreate(void);
void GameOverScreenOnOpen(GameOverScreen screen);
void GameOverScreenOnButtonClicked(GameOverScreen screen, Button button);
void GameOverScreenRender(GameOverScreen screen, int2 mousePos);
