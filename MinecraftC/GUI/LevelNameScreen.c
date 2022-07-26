#include <string.h>
#include "LevelNameScreen.h"
#include "Screen.h"
#include "../Minecraft.h"
#include "../Utilities/Log.h"

void LevelNameScreenCreate(LevelNameScreen * screen, GUIScreen * parent, char * name, int id) {
	GUIScreenCreate(screen);
	screen->type = GUIScreenTypeLevelName;
	screen->levelName.title = "Enter level name:";
	screen->levelName.counter = 0;
	screen->levelName.parent = parent;
	screen->levelName.name = StringCreate(name);
	screen->levelName.id = id;
	if (strcmp(name, "---") == 0) { StringSet(&screen->levelName.name, ""); }
}

void LevelNameScreenOnOpen(LevelNameScreen * screen) {
	screen->buttons = ListPush(screen->buttons, &(Button){ 0 });
	ButtonCreate(&screen->buttons[ListLength(screen->buttons) - 1], 0, screen->width / 2 - 100, screen->height / 4 + 120, "Save");
	screen->buttons = ListPush(screen->buttons, &(Button){ 0 });
	ButtonCreate(&screen->buttons[ListLength(screen->buttons) - 1], 1, screen->width / 2 - 100, screen->height / 4 + 144, "Cancel");
	screen->buttons[0].active = StringLength(screen->levelName.name) > 0;
}

void LevelNameScreenOnClose(LevelNameScreen * screen) {
}

void LevelNameScreenTick(LevelNameScreen * screen) {
	screen->levelName.counter++;
}

void LevelNameScreenRender(LevelNameScreen * screen, int mx, int my) {
	ScreenDrawFadingBox(0, 0, screen->width, screen->height, 0x05050060, 0x303060A0);
	ScreenDrawCenteredString(screen->font, screen->levelName.title, screen->width / 2, 40, 0xffffffff);
	int x = screen->width / 2 - 100;
	int y = screen->height / 2 - 10;
	ScreenDrawBox(x - 1, y - 1, x + 201, y + 21, 0xA0A0A0FF);
	ScreenDrawBox(x, y, x + 200, y + 20, 0x000000ff);
	String string = StringCreate(screen->levelName.name);
	StringConcat(&string, screen->levelName.counter / 6 % 2 == 0 ? "_" : "");
	ScreenDrawString(screen->font, string, x + 4, y + 6, 0xE0E0E0FF);
	StringFree(string);
}

void LevelNameScreenOnKeyPressed(LevelNameScreen * screen, char eventChar, int eventKey) {
	if (eventKey == SDL_SCANCODE_BACKSPACE && StringLength(screen->levelName.name) > 0) {
		screen->levelName.name = StringSub(screen->levelName.name, 0, StringLength(screen->levelName.name) - 2);
	}
	
	String allowedChars = StringCreate("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ,.:-_\'*!\"#%/()=+?[]{}<>");
	if (StringIndexOf(allowedChars, eventChar) >= 0 && StringLength(screen->levelName.name) < 64) {
		StringConcat(&screen->levelName.name, (char[]){ eventChar, '\0' });
	}
	StringFree(allowedChars);
	screen->buttons[0].active = StringLength(screen->levelName.name) > 0;
}

void LevelNameScreenOnButtonClicked(LevelNameScreen * screen, Button * button) {
	if (button->active) {
		if (button->id == 0 && StringLength(screen->levelName.name) > 0) {
			String filePath = StringCreate(screen->minecraft->workingDirectory);
			StringConcat(&filePath, "Level0.dat");
			filePath[StringLength(filePath) - 5] += screen->levelName.id;
			bool success = LevelSave(&screen->minecraft->level, filePath, screen->levelName.name);
			StringFree(filePath);
			if (!success) {
				LogWarning("failed to save level: %s\n", SDL_GetError());
			}
			
			MinecraftSetCurrentScreen(screen->minecraft, NULL);
			MinecraftGrabMouse(screen->minecraft);
		}
		
		if (button->id == 1) {
			MinecraftSetCurrentScreen(screen->minecraft, screen->levelName.parent);
		}
	}
}

void LevelNameScreenDestroy(LevelNameScreen * screen) {
	StringFree(screen->levelName.name);
}
