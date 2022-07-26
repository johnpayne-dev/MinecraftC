#include "PauseScreen.h"
#include "OptionsScreen.h"
#include "GenerateLevelScreen.h"
#include "LoadLevelScreen.h"
#include "SaveLevelScreen.h"
#include "Screen.h"
#include "../Minecraft.h"

void PauseScreenCreate(PauseScreen * screen) {
	GUIScreenCreate(screen);
	screen->type = GUIScreenTypePause;
}

void PauseScreenOnOpen(PauseScreen * screen) {
	screen->buttons = ListPush(screen->buttons, &(Button){ 0 });
	ButtonCreate(&screen->buttons[0], 0, screen->width / 2 - 100, screen->height / 4, "Options...");
	screen->buttons = ListPush(screen->buttons, &(Button){ 0 });
	ButtonCreate(&screen->buttons[1], 1, screen->width / 2 - 100, screen->height / 4 + 24, "Generate new level...");
	screen->buttons = ListPush(screen->buttons, &(Button){ 0 });
	ButtonCreate(&screen->buttons[2], 2, screen->width / 2 - 100, screen->height / 4 + 48, "Save level..");
	screen->buttons = ListPush(screen->buttons, &(Button){ 0 });
	ButtonCreate(&screen->buttons[3], 3, screen->width / 2 - 100, screen->height / 4 + 72, "Load level..");
	screen->buttons = ListPush(screen->buttons, &(Button){ 0 });
	ButtonCreate(&screen->buttons[4], 4, screen->width / 2 - 100, screen->height / 4 + 120, "Back to game");
}

void PauseScreenOnButtonClicked(PauseScreen * screen, Button * button) {
	if (button->id == 0) {
		OptionsScreen * options = malloc(sizeof(OptionsScreen));
		OptionsScreenCreate(options, screen, &screen->minecraft->settings);
		MinecraftSetCurrentScreen(screen->minecraft, options);
	}
	if (button->id == 1) {
		GenerateLevelScreen * generate = malloc(sizeof(GenerateLevelScreen));
		GenerateLevelScreenCreate(generate, screen);
		MinecraftSetCurrentScreen(screen->minecraft, generate);
	}
	if (button->id == 2) {
		SaveLevelScreen * saveLevel = malloc(sizeof(SaveLevelScreen));
		SaveLevelScreenCreate(saveLevel, screen);
		MinecraftSetCurrentScreen(screen->minecraft, saveLevel);
	}
	if (button->id == 3) {
		LoadLevelScreen * loadLevel = malloc(sizeof(LoadLevelScreen));
		LoadLevelScreenCreate(loadLevel, screen);
		MinecraftSetCurrentScreen(screen->minecraft, loadLevel);
	}
	if (button->id == 4) {
		MinecraftGrabMouse(screen->minecraft);
		MinecraftSetCurrentScreen(screen->minecraft, NULL);
	}
}

void PauseScreenRender(PauseScreen * screen, int mx, int my) {
	ScreenDrawFadingBox(0, 0, screen->width, screen->height, 0x05050060, 0x303060A0);
	ScreenDrawCenteredString(screen->font, "Game menu", screen->width / 2, 20, 0xffffffff);
}
