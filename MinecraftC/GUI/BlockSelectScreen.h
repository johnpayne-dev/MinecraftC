#pragma once
#include "GUIScreen.h"

typedef GUIScreen BlockSelectScreen;

BlockSelectScreen BlockSelectScreenCreate(void);
void BlockSelectScreenRender(BlockSelectScreen screen, int mx, int my);
void BlockSelectScreenOnMouseClicked(BlockSelectScreen screen, int x, int y, int button);
