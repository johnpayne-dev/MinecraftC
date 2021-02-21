#pragma once
#include "GUIScreen.h"

typedef GUIScreen GenerateLevelScreen;

typedef struct GenerateLevelScreenData
{
	GUIScreen Parent;
} * GenerateLevelScreenData;

GenerateLevelScreen GenerateLevelScreenCreate(GUIScreen parent);
void GenerateLevelScreenOnOpen(GenerateLevelScreen screen);
void GenerateLevelScreenOnButtonClicked(GenerateLevelScreen screen, Button button);
void GenerateLevelScreenRender(GenerateLevelScreen screen, int2 mousePos);
void GenerateLevelScreenDestroy(GenerateLevelScreen screen);
