#include <string.h>
#include "LoadLevelScreen.h"
#include "SaveLevelScreen.h"
#include "../Minecraft.h"
#include "Screen.h"
#include "../Utilities/Log.h"

void LoadLevelScreenCreate(LoadLevelScreen * screen, GUIScreen * parent) {
	GUIScreenCreate(screen);
	screen->type = GUIScreenTypeLoadLevel;
	screen->level.parent = parent;
	screen->level.title = "Load level";
	for (int i = 0; i < 5; i++) {
		screen->level.names[i] = StringCreate("");
		
		String filePath = StringCreate(parent->minecraft->workingDirectory);
		StringConcat(&filePath, "Level0.dat");
		filePath[StringLength(filePath) - 5] += i;
		SDL_RWops * file = SDL_RWFromFile(filePath, "rb");
		if (file == NULL) {
			continue;
		}
		char nameChar;
		do {
			SDL_RWread(file, &nameChar, 1, 1);
			StringConcat(&screen->level.names[i], (char []){ nameChar, '\0' });
		} while (nameChar != '\0');
		
		SDL_RWclose(file);
		StringFree(filePath);
	}
}

void LoadLevelScreenOnOpen(LoadLevelScreen * screen) {
	for (int i = 0; i < 5; i++) {
		char * name = screen->level.names[i];
		if (name[0] == '\0') {
			name = "---";
		}
		screen->buttons = ListPush(screen->buttons, &(Button){ 0 });
		ButtonCreate(&screen->buttons[i], i, screen->width / 2 - 100, screen->height / 6 + i * 24, name);
		screen->buttons[i].active = screen->type != GUIScreenTypeLoadLevel || screen->level.names[i][0] != '\0';
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

void LoadLevelScreenOpenLevel(LoadLevelScreen * screen, int id) {
	if (screen->type == GUIScreenTypeSaveLevel) {
		SaveLevelScreenOpenLevel(screen, id);
		return;
	}
	String filePath = StringCreate(screen->minecraft->workingDirectory);
	StringConcat(&filePath, "Level0.dat");
	filePath[StringLength(filePath) - 5] += id;
	LevelLoad(&screen->minecraft->level, filePath);
	StringFree(filePath);
	
	MinecraftSetCurrentScreen(screen->minecraft, NULL);
	MinecraftGrabMouse(screen->minecraft);
}

void LoadLevelScreenOpenLevelFromFile(LoadLevelScreen * screen, char * file) {
	if (screen->type == GUIScreenTypeSaveLevel) { SaveLevelScreenOpenLevelFromFile(screen, file); return; }
	MinecraftSetCurrentScreen(screen->minecraft, screen->level.parent);
}

void LoadLevelScreenRender(LoadLevelScreen * screen, int mx, int my) {
	ScreenDrawFadingBox(0, 0, screen->width, screen->height, 0x05050060, 0x303060A0);
	ScreenDrawCenteredString(screen->font, screen->level.title, screen->width / 2, 20, 0xffffffff);
	GUIScreenType type = screen->type;
	screen->type = GUIScreenTypeNone;
	GUIScreenRender(screen, mx, my);
	screen->type = type;
}
