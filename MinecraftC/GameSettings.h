#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "KeyBinding.h"
#include "Utilities/List.h"
#include "Utilities/String.h"

typedef struct GameSettings
{
	bool Music;
	bool Sound;
	bool InvertMouse;
	bool ShowFrameRate;
	int ViewDistance;
	bool ViewBobbing;
	bool Anaglyph;
	bool LimitFramerate;
	KeyBinding ForwardKey;
	KeyBinding LeftKey;
	KeyBinding BackKey;
	KeyBinding RightKey;
	KeyBinding JumpKey;
	KeyBinding BuildKey;
	KeyBinding ChatKey;
	KeyBinding ToggleFogKey;
	KeyBinding SaveLocationKey;
	KeyBinding LoadLocationKey;
	list(KeyBinding *) Bindings;
	struct Minecraft * Minecraft;
	String File;
	int SettingsCount;
} * GameSettings;

GameSettings GameSettingsCreate(struct Minecraft * minecraft);
String GameSettingsGetBinding(GameSettings settings, int binding);
void GameSettingsSetBinding(GameSettings settings, int binding, int key);
void GameSettingsToggleSetting(GameSettings settings, int setting, int var2);
String GameSettingsGetSetting(GameSettings settings, int setting);
void GameSettingsDestroy(GameSettings settings);
