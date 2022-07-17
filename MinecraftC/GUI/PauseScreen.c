#include "PauseScreen.h"
#include "OptionsScreen.h"
#include "GenerateLevelScreen.h"
#include "LoadLevelScreen.h"
#include "SaveLevelScreen.h"
#include "Screen.h"
#include "../Minecraft.h"

PauseScreen PauseScreenCreate() {
	GUIScreen screen = GUIScreenCreate();
	screen->type = GUIScreenTypePause;
	return screen;
}

void PauseScreenOnOpen(PauseScreen screen) {
	for (int i = 0; i < ListCount(screen->buttons); i++) { ButtonDestroy(screen->buttons[i]); }
	screen->buttons = ListClear(screen->buttons);
	screen->buttons = ListPush(screen->buttons, &(Button){ ButtonCreate(0, screen->width / 2 - 100, screen->height / 4, "Options...") });
	screen->buttons = ListPush(screen->buttons, &(Button){ ButtonCreate(1, screen->width / 2 - 100, screen->height / 4 + 24, "Generate new level...") });
	screen->buttons = ListPush(screen->buttons, &(Button){ ButtonCreate(2, screen->width / 2 - 100, screen->height / 4 + 48, "Save level..") });
	screen->buttons = ListPush(screen->buttons, &(Button){ ButtonCreate(3, screen->width / 2 - 100, screen->height / 4 + 72, "Load level..") });
	screen->buttons = ListPush(screen->buttons, &(Button){ ButtonCreate(4, screen->width / 2 - 100, screen->height / 4 + 120, "Back to game") });
	
	screen->buttons[1]->active = true;
	screen->buttons[2]->active = false;
	screen->buttons[3]->active = false;
}

void PauseScreenOnButtonClicked(PauseScreen screen, Button button) {
	if (button->id == 0) { MinecraftSetCurrentScreen(screen->minecraft, OptionsScreenCreate(screen, screen->minecraft->settings)); }
	if (button->id == 1) { MinecraftSetCurrentScreen(screen->minecraft, GenerateLevelScreenCreate(screen)); }
	if (screen->minecraft->session != NULL) {
		if (button->id == 2) { MinecraftSetCurrentScreen(screen->minecraft, SaveLevelScreenCreate(screen)); }
		if (button->id == 3) { MinecraftSetCurrentScreen(screen->minecraft, LoadLevelScreenCreate(screen)); }
	}
	if (button->id == 4) {
		MinecraftGrabMouse(screen->minecraft);
		MinecraftSetCurrentScreen(screen->minecraft, NULL);
	}
}

void PauseScreenRender(PauseScreen screen, int mx, int my) {
	ScreenDrawFadingBox(0, 0, screen->width, screen->height, 0x05050060, 0x303060A0);
	ScreenDrawCenteredString(screen->font, "Game menu", screen->width / 2, 20, 0xffffffff);
}
