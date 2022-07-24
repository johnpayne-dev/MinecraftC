#include "SaveLevelScreen.h"
#include "LevelNameScreen.h"
#include "Screen.h"
#include "../Minecraft.h"

void SaveLevelScreenCreate(SaveLevelScreen * screen, GUIScreen * parent) {
	LoadLevelScreenCreate(screen, parent);
	screen->type = GUIScreenTypeSaveLevel;
	screen->level.title = "Save level";
	screen->level.saving = true;
	screen->level.parent = parent;
}

void SaveLevelScreenSetLevels(SaveLevelScreen * screen, char * strings[5]) {
	for (int i = 0; i < 5; i++) {
		StringSet(&screen->buttons[i].text, strings[i]);
		screen->buttons[i].visible = true;
		screen->buttons[i].active = true;
	}
}

void SaveLevelScreenOnOpen(SaveLevelScreen * screen) {
	StringSet(&screen->buttons[5].text, "Save file...");
}

void SaveLevelScreenOpenLevel(SaveLevelScreen * screen, int level) {
	LevelNameScreen * levelName = malloc(sizeof(LevelNameScreen));
	LevelNameScreenCreate(levelName, screen, screen->buttons[level].text, level);
	MinecraftSetCurrentScreen(screen->minecraft, levelName);
}

void SaveLevelScreenOpenLevelFromFile(SaveLevelScreen * screen, char * file) {
	LevelIOSave(&screen->minecraft->levelIO, &screen->minecraft->level, SDL_RWFromFile(file, "wb"));
	MinecraftSetCurrentScreen(screen->minecraft, screen->level.parent);
}
