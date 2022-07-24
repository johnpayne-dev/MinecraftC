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
	screen->level.status = "";
	screen->level.finished = false;
	screen->level.loaded = false;
	screen->level.frozen = false;
	screen->level.saving = false;
}

void LoadLevelScreenRun(LoadLevelScreen * screen) {
	screen->level.status = "Failed to load levels";
	screen->level.finished = true;
}

void LoadLevelScreenSetLevels(LoadLevelScreen * screen, char * levels[5]) {
	if (screen->type == GUIScreenTypeSaveLevel) { SaveLevelScreenSetLevels(screen, levels); return; }
	for (int i = 0; i < 5; i++) {
		screen->buttons[i].active = !(strcmp(levels[i], "-") == 0);
		StringSet(&screen->buttons[i].text, levels[i]);
		screen->buttons[i].visible = true;
	}
}

void LoadLevelScreenOnOpen(LoadLevelScreen * screen) {
	LoadLevelScreenRun(screen);
	for (int i = 0; i < 5; i++) {
		screen->buttons = ListPush(screen->buttons, &(Button){ 0 });
		ButtonCreate(&screen->buttons[i], i, screen->width / 2 - 100, screen->height / 6 + i * 24, "---");
		screen->buttons[i].visible = false;
		screen->buttons[i].active = false;
	}
	screen->buttons = ListPush(screen->buttons, &(Button){ 0 });
	ButtonCreate(&screen->buttons[ListLength(screen->buttons) - 1], 5, screen->width / 2 - 100, screen->height / 6 + 132, "Load file...");
	screen->buttons = ListPush(screen->buttons, &(Button){ 0 });
	ButtonCreate(&screen->buttons[ListLength(screen->buttons) - 1], 6, screen->width / 2 - 100, screen->height / 6 + 168, "Cancel");
	if (screen->type == GUIScreenTypeSaveLevel) { SaveLevelScreenOnOpen(screen); return; }
}

void LoadLevelScreenOnButtonClicked(LoadLevelScreen * screen, Button * button) {
	if (!screen->level.frozen && button->active) {
		if (screen->level.loaded && button->id < 5) { LoadLevelScreenOpenLevel(screen, button->id); }
		
		if (screen->level.finished || (screen->level.loaded && button->id == 5)) {
			screen->level.frozen = true;
			/*
			 LevelDialog var2;
			 (var2 = new LevelDialog(this)).setDaemon(true);
			 SwingUtilities.invokeLater(var2);
			 */
		}
		
		if (screen->level.finished || (screen->level.loaded && button->id == 6)) {
			MinecraftSetCurrentScreen(screen->minecraft, screen->level.parent);
		}
	}
}

void LoadLevelScreenOpenLevel(LoadLevelScreen * screen, int level) {
	if (screen->type == GUIScreenTypeSaveLevel) { SaveLevelScreenOpenLevel(screen, level); return; }
	MinecraftSetCurrentScreen(screen->minecraft, NULL);
	MinecraftGrabMouse(screen->minecraft);
}

void LoadLevelScreenOpenLevelFromFile(LoadLevelScreen * screen, char * file) {
	if (screen->type == GUIScreenTypeSaveLevel) { SaveLevelScreenOpenLevelFromFile(screen, file); return; }
	Level * level = LevelIOLoad(&screen->minecraft->levelIO, SDL_RWFromFile(file, "rb"));
	MinecraftSetCurrentScreen(screen->minecraft, screen->level.parent);
}

void LoadLevelScreenOnClose(LoadLevelScreen * screen) {
	/*
	 super.onClose();
	       if(this.chooser != null) {
		  this.chooser.cancelSelection();
	       }
	 */
}

void LoadLevelScreenTick(LoadLevelScreen * screen) {
	if (screen->level.selectedFile != NULL) {
		LoadLevelScreenOpenLevelFromFile(screen, screen->level.selectedFile);
		screen->level.selectedFile = NULL;
	}
}

void LoadLevelScreenRender(LoadLevelScreen * screen, int mx, int my) {
	ScreenDrawFadingBox(0, 0, screen->width, screen->height, 0x05050060, 0x303060A0);
	ScreenDrawCenteredString(screen->font, screen->level.title, screen->width / 2, 20, 0xffffffff);
	if (screen->level.frozen) {
		ScreenDrawCenteredString(screen->font, "Selecting file..", screen->width / 2, screen->height / 2 - 4, 0xffffffff);
	} else {
		if (!screen->level.loaded) {
			ScreenDrawCenteredString(screen->font, screen->level.status, screen->width / 2, screen->height / 2 - 4, 0xffffffff);
		}
		screen->type = GUIScreenTypeNone;
		GUIScreenRender(screen, mx, my);
		screen->type = GUIScreenTypeLoadLevel;
	}
}
