#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <SDL.h>
#include "Utilities/List.h"
#include "Utilities/String.h"

typedef struct KeyBinding {
	char * name;
	int key;
} KeyBinding;

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
	List(KeyBinding *) bindings;
#if MINECRAFTC_MODS
	bool explodingTNT;
	bool raytracing;
	int modsCount;
#endif
	struct Minecraft * minecraft;
	String file;
	int settingsCount;
} GameSettings;

void GameSettingsCreate(GameSettings * settings, struct Minecraft * minecraft);
String GameSettingsGetBinding(GameSettings * settings, int binding);
void GameSettingsSetBinding(GameSettings * settings, int binding, int key);
void GameSettingsToggleSetting(GameSettings * settings, int setting);
String GameSettingsGetSetting(GameSettings * settings, int setting);
void GameSettingsDestroy(GameSettings * settings);
