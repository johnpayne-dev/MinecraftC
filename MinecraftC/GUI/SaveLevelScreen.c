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
		StringSet(&screen->buttons[i]->text, strings[i]);
		screen->buttons[i]->visible = true;
		screen->buttons[i]->active = true;
	}
}

void SaveLevelScreenOnOpen(SaveLevelScreen screen) {
	StringSet(&screen->buttons[5]->text, "Save file...");
}

void SaveLevelScreenOpenLevel(SaveLevelScreen screen, int level) {
	MinecraftSetCurrentScreen(screen->minecraft, LevelNameScreenCreate(screen, screen->buttons[level]->text, level));
}

void SaveLevelScreenOpenLevelFromFile(SaveLevelScreen screen, char * file) {
	LoadLevelScreenData this = screen->typeData;
	LevelIOSave(&screen->minecraft->levelIO, screen->minecraft->level, SDL_RWFromFile(file, "wb"));
	MinecraftSetCurrentScreen(screen->minecraft, this->parent);
}
