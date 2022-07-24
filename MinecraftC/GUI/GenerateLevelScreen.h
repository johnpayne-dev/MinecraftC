#pragma once
#include "GUIScreen.h"

typedef GUIScreen GenerateLevelScreen;

typedef struct GenerateLevelScreenData GenerateLevelScreenData;

void GenerateLevelScreenCreate(GenerateLevelScreen * screen, GUIScreen * parent);
void GenerateLevelScreenOnOpen(GenerateLevelScreen * screen);
void GenerateLevelScreenOnButtonClicked(GenerateLevelScreen * screen, Button * button);
void GenerateLevelScreenRender(GenerateLevelScreen * screen, int mx, int my);
