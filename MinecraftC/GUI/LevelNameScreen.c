#include <string.h>
#include "LevelNameScreen.h"
#include "Screen.h"
#include "../Minecraft.h"

LevelNameScreen LevelNameScreenCreate(GUIScreen parent, char * name, int id) {
	GUIScreen screen = GUIScreenCreate();
	screen->type = GUIScreenTypeLevelName;
	screen->typeData = MemoryAllocate(sizeof(struct LevelNameScreenData));
	LevelNameScreenData this = screen->typeData;
	this->title = "Enter level name:";
	this->counter = 0;
	this->parent = parent;
	this->name = StringCreate(name);
	if (strcmp(name, "-") == 0) { this->name = StringSet(this->name, ""); }
	return screen;
}

void LevelNameScreenOnOpen(LevelNameScreen screen) {
	LevelNameScreenData this = screen->typeData;
	for (int i = 0; i < ListCount(screen->buttons); i++) { ButtonDestroy(screen->buttons[i]); }
	screen->buttons = ListClear(screen->buttons);
	//Keyboard.enableRepeatEvents(true);
	screen->buttons = ListPush(screen->buttons, &(Button){ ButtonCreate(0, screen->width / 2 - 100, screen->height / 4 + 120, "Save") });
	screen->buttons = ListPush(screen->buttons, &(Button){ ButtonCreate(0, screen->width / 2 - 100, screen->height / 4 + 144, "Cancel") });
	screen->buttons[0]->active = StringLength(this->name) > 0;
}

void LevelNameScreenOnClose(LevelNameScreen screen) {
	//Keyboard.enableRepeatEvents(false);
}

void LevelNameScreenTick(LevelNameScreen screen) {
	LevelNameScreenData this = screen->typeData;
	this->counter++;
}

void LevelNameScreenRender(LevelNameScreen screen, int2 mousePos) {
	LevelNameScreenData this = screen->typeData;
	ScreenDrawFadingBox((int2){ 0, 0 }, (int2){ screen->width, screen->height }, ColorFromHex(0x05050060), ColorFromHex(0x303060A0));
	ScreenDrawCenteredString(screen->font, this->title, (int2){ screen->width / 2, 40 }, ColorWhite);
	int x = screen->width / 2 - 100;
	int y = screen->height / 2 - 10;
	ScreenDrawBox((int2){ x - 1, y - 1 }, (int2){ x + 201, y + 21 }, ColorFromHex(0xA0A0A0FF));
	ScreenDrawBox((int2){ x, y }, (int2){ x + 200, y + 20 }, ColorBlack);
	String string = StringConcat(StringCreate(this->name), this->counter / 6 % 2 == 0 ? "_" : "");
	ScreenDrawString(screen->font, string, (int2){ x + 4, y + 6 }, ColorFromHex(0xE0E0E0FF));
	StringDestroy(string);
}

void LevelNameScreenOnKeyPressed(LevelNameScreen screen, char eventChar, int eventKey) {
	LevelNameScreenData this = screen->typeData;
	if (eventKey == 14 && StringLength(this->name) > 0) { this->name = StringSub(this->name, 0, StringLength(this->name) - 1); }
	
	String allowedChars = StringCreate("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ,.:-_\'*!\"#%/()=+?[]{}<>");
	if (StringIndexOf(allowedChars, eventChar) >= 0 && StringLength(this->name) < 64) {
		this->name = StringConcat(this->name, (char[]){ eventChar, '\0' });
	}
	
	screen->buttons[0]->active = StringLength(this->name) > 0;
}

void LevelNameScreenOnButtonClicked(LevelNameScreen screen, Button button) {
	LevelNameScreenData this = screen->typeData;
	if (button->active) {
		if (button->id == 0 && StringLength(this->name) > 0) {
			LevelIOSaveOnline(screen->minecraft->levelIO, screen->minecraft->level, screen->minecraft->host, screen->minecraft->session->userName, screen->minecraft->session->sessionID, this->name, this->id);
			MinecraftSetCurrentScreen(screen->minecraft, NULL);
			MinecraftGrabMouse(screen->minecraft);
		}
		
		if (button->id == 1) {
			MinecraftSetCurrentScreen(screen->minecraft, this->parent);
		}
	}
}

void LevelNameScreenDestroy(LevelNameScreen screen) {
	LevelNameScreenData this = screen->typeData;
	MemoryFree(this);
}
