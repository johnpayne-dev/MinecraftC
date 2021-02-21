#pragma once
#include "../ChatLine.h"
#include "../Utilities/List.h"
#include "../Utilities/Random.h"

typedef struct HUDScreen
{
	list(ChatLine) Chat;
	RandomGenerator Random;
	struct Minecraft * Minecraft;
	int Width, Height;
	const char * HoveredPlayer;
	int Ticks;
} * HUDScreen;

HUDScreen HUDScreenCreate(struct Minecraft * minecraft, int width, int height);
void HUDScreenRender(HUDScreen hud, float var1, bool var2, int2 mousePos);
void HUDScreenAddChat(HUDScreen hud, char * message);
void HUDScreenDestroy(HUDScreen hud);
