#pragma once
#include "../ChatLine.h"
#include "../Utilities/List.h"
#include "../Utilities/Random.h"
#include <stdbool.h>

typedef struct HUDScreen {
	list(ChatLine) chat;
	RandomGenerator random;
	struct Minecraft * minecraft;
	int width, height;
	const char * hoveredPlayer;
	int ticks;
} * HUDScreen;

HUDScreen HUDScreenCreate(struct Minecraft * minecraft, int width, int height);
void HUDScreenRender(HUDScreen hud, float dt, int mx, int my);
void HUDScreenAddChat(HUDScreen hud, char * message);
void HUDScreenDestroy(HUDScreen hud);
