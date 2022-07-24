#pragma once
#include "GUIScreen.h"

typedef GUIScreen BlockSelectScreen;

void BlockSelectScreenCreate(BlockSelectScreen * screen);
void BlockSelectScreenRender(BlockSelectScreen * screen, int mx, int my);
void BlockSelectScreenOnMouseClicked(BlockSelectScreen * screen, int x, int y, int button);
