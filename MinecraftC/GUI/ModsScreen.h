#pragma once
#if MINECRAFTC_MODS
#include "GUIScreen.h"

typedef GUIScreen ModsScreen;

typedef struct ModsScreenData ModsScreenData;

void ModsScreenCreate(ModsScreen * screen, GUIScreen * parent, GameSettings * settings);
void ModsScreenOnOpen(ModsScreen * screen);
void ModsScreenRender(ModsScreen * screen, int mx, int my);
void ModsScreenOnButtonClicked(ModsScreen * screen, Button * button);

#endif
