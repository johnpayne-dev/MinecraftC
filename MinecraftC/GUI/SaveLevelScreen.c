#include "SaveLevelScreen.h"
#include "LevelNameScreen.h"
#include "Screen.h"
#include "../Minecraft.h"

void SaveLevelScreenCreate(SaveLevelScreen * screen, GUIScreen * parent) {
	LoadLevelScreenCreate(screen, parent);
	screen->type = GUIScreenTypeSaveLevel;
	screen->level.title = "Save level";
	screen->level.parent = parent;
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
	MinecraftSetCurrentScreen(screen->minecraft, screen->level.parent);
}
