#include "SaveLevelScreen.h"
#include "LevelNameScreen.h"
#include "Screen.h"
#include "../Minecraft.h"

SaveLevelScreen SaveLevelScreenCreate(GUIScreen parent) {
	GUIScreen screen = LoadLevelScreenCreate(parent);
	screen->type = GUIScreenTypeSaveLevel;
	LoadLevelScreenData this = screen->typeData;
	this->title = "Save level";
	this->saving = true;
	this->parent = parent;
	return screen;
}

void SaveLevelScreenSetLevels(SaveLevelScreen screen, char * strings[5]) {
	for (int i = 0; i < 5; i++) {
		screen->buttons[i]->text = StringSet(screen->buttons[i]->text, strings[i]);
		screen->buttons[i]->visible = true;
		screen->buttons[i]->active = screen->minecraft->session->hasPaid;
	}
}

void SaveLevelScreenOnOpen(SaveLevelScreen screen) {
	screen->buttons[5]->text = StringSet(screen->buttons[5]->text, "Save file...");
}

void SaveLevelScreenOpenLevel(SaveLevelScreen screen, int level) {
	MinecraftSetCurrentScreen(screen->minecraft, LevelNameScreenCreate(screen, screen->buttons[level]->text, level));
}

void SaveLevelScreenOpenLevelFromFile(SaveLevelScreen screen, char * file) {
	LoadLevelScreenData this = screen->typeData;
	LevelIOSave(screen->minecraft->levelIO, screen->minecraft->level, SDL_RWFromFile(file, "wb"));
	MinecraftSetCurrentScreen(screen->minecraft, this->parent);
}

void SaveLevelScreenRender(SaveLevelScreen screen, int2 mousePos) {
	if (!screen->minecraft->session->hasPaid) {
		ScreenDrawFadingBox((int2){ screen->width / 2 - 80, 72 }, (int2){ screen->width / 2 + 80, 120 }, ColorFromHex(0x000000E0), ColorFromHex(0x000000E0));
		ScreenDrawCenteredString(screen->font, "Premium only!", (int2){ screen->width / 2, 80 }, ColorFromHex(0xFF9090FF));
		ScreenDrawCenteredString(screen->font, "Purchase the game to be able", (int2){ screen->width / 2, 96 }, ColorFromHex(0xE08080FF));
		ScreenDrawCenteredString(screen->font, "to save your levels online.", (int2){ screen->width / 2, 104 }, ColorFromHex(0xE08080FF));
	}
}
