#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "KeyBinding.h"
#include "Utilities/List.h"
#include "Utilities/String.h"

typedef struct GameSettings {
	bool music;
	bool sound;
	bool invertMouse;
	bool showFrameRate;
	int viewDistance;
	bool viewBobbing;
	bool anaglyph;
	bool limitFramerate;
	KeyBinding forwardKey;
	KeyBinding leftKey;
	KeyBinding backKey;
	KeyBinding rightKey;
	KeyBinding jumpKey;
	KeyBinding buildKey;
	KeyBinding chatKey;
	KeyBinding toggleFogKey;
	KeyBinding saveLocationKey;
	KeyBinding loadLocationKey;
	list(KeyBinding *) bindings;
	struct Minecraft * minecraft;
	String file;
	int settingsCount;
} * GameSettings;

GameSettings GameSettingsCreate(struct Minecraft * minecraft);
String GameSettingsGetBinding(GameSettings settings, int binding);
void GameSettingsSetBinding(GameSettings settings, int binding, int key);
void GameSettingsToggleSetting(GameSettings settings, int setting, int var2);
String GameSettingsGetSetting(GameSettings settings, int setting);
void GameSettingsDestroy(GameSettings settings);
