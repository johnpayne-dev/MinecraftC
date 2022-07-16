#pragma once
#include "../ChatLine.h"
#include "../Utilities/List.h"
#include "../Utilities/Random.h"

typedef struct HUDScreen {
	list(ChatLine) chat;
	RandomGenerator random;
	struct Minecraft * minecraft;
	int width, height;
	const char * hoveredPlayer;
	int ticks;
} * HUDScreen;

HUDScreen HUDScreenCreate(struct Minecraft * minecraft, int width, int height);
void HUDScreenRender(HUDScreen hud, float var1, bool var2, int2 mousePos);
void HUDScreenAddChat(HUDScreen hud, char * message);
void HUDScreenDestroy(HUDScreen hud);
