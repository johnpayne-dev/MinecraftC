#include <string.h>
#include "LoadLevelScreen.h"
#include "SaveLevelScreen.h"
#include "../Minecraft.h"
#include "Screen.h"

void LoadLevelScreenCreate(LoadLevelScreen * screen, GUIScreen * parent) {
	GUIScreenCreate(screen);
	screen->type = GUIScreenTypeLoadLevel;
	screen->level.parent = parent;
	screen->level.title = "Load level";
}

void LoadLevelScreenOnOpen(LoadLevelScreen * screen) {
	for (int i = 0; i < 5; i++) {
		String name = StringCreateFromInt(i);
		StringConcatFront("Level", &name);
		screen->buttons = ListPush(screen->buttons, &(Button){ 0 });
		ButtonCreate(&screen->buttons[i], i, screen->width / 2 - 100, screen->height / 6 + i * 24, name);
		StringFree(name);
		screen->buttons[i].active = false;
	}
	screen->buttons = ListPush(screen->buttons, &(Button){ 0 });
	ButtonCreate(&screen->buttons[ListLength(screen->buttons) - 1], 5, screen->width / 2 - 100, screen->height / 6 + 132, "Load file...");
	screen->buttons[5].active = false;
	screen->buttons = ListPush(screen->buttons, &(Button){ 0 });
	ButtonCreate(&screen->buttons[ListLength(screen->buttons) - 1], 6, screen->width / 2 - 100, screen->height / 6 + 168, "Cancel");
	if (screen->type == GUIScreenTypeSaveLevel) { SaveLevelScreenOnOpen(screen); return; }
}

void LoadLevelScreenOnButtonClicked(LoadLevelScreen * screen, Button * button) {
	if (button->active) {
		if (button->id < 5) {
			LoadLevelScreenOpenLevel(screen, button->id);
		} else if (button->id == 5) {
		} else if (button->id == 6) {
			MinecraftSetCurrentScreen(screen->minecraft, screen->level.parent);
		}
	}
}

void LoadLevelScreenOpenLevel(LoadLevelScreen * screen, int level) {
	if (screen->type == GUIScreenTypeSaveLevel) {
		SaveLevelScreenOpenLevel(screen, level);
		return;
	}
	MinecraftSetCurrentScreen(screen->minecraft, NULL);
	MinecraftGrabMouse(screen->minecraft);
}

void LoadLevelScreenOpenLevelFromFile(LoadLevelScreen * screen, char * file) {
	if (screen->type == GUIScreenTypeSaveLevel) { SaveLevelScreenOpenLevelFromFile(screen, file); return; }
	Level * level = LevelIOLoad(&screen->minecraft->levelIO, SDL_RWFromFile(file, "rb"));
	MinecraftSetCurrentScreen(screen->minecraft, screen->level.parent);
}

void LoadLevelScreenRender(LoadLevelScreen * screen, int mx, int my) {
	ScreenDrawFadingBox(0, 0, screen->width, screen->height, 0x05050060, 0x303060A0);
	ScreenDrawCenteredString(screen->font, screen->level.title, screen->width / 2, 20, 0xffffffff);
	screen->type = GUIScreenTypeNone;
	GUIScreenRender(screen, mx, my);
	screen->type = GUIScreenTypeLoadLevel;
}
