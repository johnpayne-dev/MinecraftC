#include <OpenGL.h>
#include "GameOverScreen.h"
#include "OptionsScreen.h"
#include "GenerateLevelScreen.h"
#include "LoadLevelScreen.h"
#include "Screen.h"
#include "../Minecraft.h"

GameOverScreen GameOverScreenCreate()
{
	GUIScreen screen = GUIScreenCreate();
	screen->Type = GUIScreenTypeGameOver;
	return screen;
}

void GameOverScreenOnOpen(GameOverScreen screen)
{
	for (int i = 0; i < ListCount(screen->Buttons); i++) { ButtonDestroy(screen->Buttons[i]); }
	screen->Buttons = ListClear(screen->Buttons);
	screen->Buttons = ListPush(screen->Buttons, &(Button){ ButtonCreate(1, screen->Width / 2 - 100, screen->Height / 4 + 72, "Generate new level...") });
	screen->Buttons = ListPush(screen->Buttons, &(Button){ ButtonCreate(2, screen->Width / 2 - 100, screen->Height / 4 + 96, "Load level...") });
	if (screen->Minecraft->Session == NULL) { screen->Buttons[1]->Active = false; }
}

void GameOverScreenOnButtonClicked(GameOverScreen screen, Button button)
{
	if (button->ID == 0) { MinecraftSetCurrentScreen(screen->Minecraft, OptionsScreenCreate(screen, screen->Minecraft->Settings)); }
	if (button->ID == 1) { MinecraftSetCurrentScreen(screen->Minecraft, GenerateLevelScreenCreate(screen)); }
	if (screen->Minecraft->Session != NULL && button->ID == 2) { MinecraftSetCurrentScreen(screen->Minecraft, LoadLevelScreenCreate(screen)); }
}

void GameOverScreenRender(GameOverScreen screen, int2 mousePos)
{
	ScreenDrawFadingBox((int2){ 0, 0 }, (int2){ screen->Width, screen->Height }, ColorFromHex(0x50000060), ColorFromHex(0x803030A0));
	glPushMatrix();
	glScalef(2.0, 2.0, 2.0);
	ScreenDrawCenteredString(screen->Font, "Game over!", (int2){ screen->Width / 4, 30 }, ColorWhite);
	glPopMatrix();
	String score = StringConcatFront("Score: &e", StringCreateFromInt(PlayerGetScore(screen->Minecraft->Player)));
	ScreenDrawCenteredString(screen->Font, score, (int2){ screen->Width / 2, 100 }, ColorWhite);
	StringDestroy(score);
}
