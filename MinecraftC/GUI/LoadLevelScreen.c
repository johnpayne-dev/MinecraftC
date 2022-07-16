#include <string.h>
#include "LoadLevelScreen.h"
#include "SaveLevelScreen.h"
#include "../Minecraft.h"
#include "Screen.h"

LoadLevelScreen LoadLevelScreenCreate(GUIScreen parent) {
	GUIScreen screen = GUIScreenCreate();
	screen->type = GUIScreenTypeLoadLevel;
	screen->typeData = MemoryAllocate(sizeof(struct LoadLevelScreenData));
	LoadLevelScreenData this = screen->typeData;
	this->parent = parent;
	this->title = "Load level";
	this->status = "";
	this->finished = false;
	this->loaded = false;
	this->frozen = false;
	this->saving = false;
	return screen;
}

void LoadLevelScreenRun(LoadLevelScreen screen) {
	LoadLevelScreenData this = screen->typeData;
	this->status = "Failed to load levels";
	this->finished = true;
}

void LoadLevelScreenSetLevels(LoadLevelScreen screen, char * levels[5]) {
	if (screen->type == GUIScreenTypeSaveLevel) { SaveLevelScreenSetLevels(screen, levels); return; }
	for (int i = 0; i < 5; i++) {
		screen->buttons[i]->active = !(strcmp(levels[i], "-") == 0);
		screen->buttons[i]->text = StringSet(screen->buttons[i]->text, levels[i]);
		screen->buttons[i]->visible = true;
	}
}

void LoadLevelScreenOnOpen(LoadLevelScreen screen) {
	LoadLevelScreenRun(screen);
	for (int i = 0; i < 5; i++) {
		screen->buttons = ListPush(screen->buttons, &(Button){ ButtonCreate(i, screen->width / 2 - 100, screen->height / 6 + i * 24, "---") });
		screen->buttons[i]->visible = false;
		screen->buttons[i]->active = false;
	}
	screen->buttons = ListPush(screen->buttons, &(Button){ ButtonCreate(5, screen->width / 2 - 100, screen->height / 6 + 132, "Load file...") });
	screen->buttons = ListPush(screen->buttons, &(Button){ ButtonCreate(6, screen->width / 2 - 100, screen->height / 6 + 168, "Cancel") });
	if (screen->type == GUIScreenTypeSaveLevel) { SaveLevelScreenOnOpen(screen); return; }
}

void LoadLevelScreenOnButtonClicked(LoadLevelScreen screen, Button button) {
	LoadLevelScreenData this = screen->typeData;
	if (!this->frozen && button->active) {
		if (this->loaded && button->id < 5) { LoadLevelScreenOpenLevel(screen, button->id); }
		
		if (this->finished || (this->loaded && button->id == 5)) {
			this->frozen = true;
			/*
			 LevelDialog var2;
			 (var2 = new LevelDialog(this)).setDaemon(true);
			 SwingUtilities.invokeLater(var2);
			 */
		}
		
		if (this->finished || (this->loaded && button->id == 6)) {
			MinecraftSetCurrentScreen(screen->minecraft, this->parent);
		}
	}
}

void LoadLevelScreenOpenLevel(LoadLevelScreen screen, int level) {
	if (screen->type == GUIScreenTypeSaveLevel) { SaveLevelScreenOpenLevel(screen, level); return; }
	MinecraftLoadOnlineLevel(screen->minecraft, screen->minecraft->session->userName, level);
	MinecraftSetCurrentScreen(screen->minecraft, NULL);
	MinecraftGrabMouse(screen->minecraft);
}

void LoadLevelScreenOpenLevelFromFile(LoadLevelScreen screen, char * file) {
	if (screen->type == GUIScreenTypeSaveLevel) { SaveLevelScreenOpenLevelFromFile(screen, file); return; }
	LoadLevelScreenData this = screen->typeData;
	Level level = LevelIOLoad(screen->minecraft->levelIO, SDL_RWFromFile(file, "rb"));
	if (level != NULL) { MinecraftSetLevel(screen->minecraft, level); }
	MinecraftSetCurrentScreen(screen->minecraft, this->parent);
}

void LoadLevelScreenOnClose(LoadLevelScreen screen) {
	/*
	 super.onClose();
	       if(this.chooser != null) {
		  this.chooser.cancelSelection();
	       }
	 */
}

void LoadLevelScreenTick(LoadLevelScreen screen) {
	LoadLevelScreenData this = screen->typeData;
	if (this->selectedFile != NULL)
	{
		LoadLevelScreenOpenLevelFromFile(screen, this->selectedFile);
		this->selectedFile = NULL;
	}
}

void LoadLevelScreenRender(LoadLevelScreen screen, int2 mousePos) {
	LoadLevelScreenData this = screen->typeData;
	ScreenDrawFadingBox((int2){ 0, 0 }, (int2){ screen->width, screen->height }, ColorFromHex(0x05050060), ColorFromHex(0x303060A0));
	ScreenDrawCenteredString(screen->font, this->title, (int2){ screen->width / 2, 20 }, ColorWhite);
	if (this->frozen) {
		ScreenDrawCenteredString(screen->font, "Selecting file..", (int2){ screen->width / 2, screen->height / 2 - 4 }, ColorWhite);
	} else {
		if (!this->loaded) {
			ScreenDrawCenteredString(screen->font, this->status, (int2){ screen->width / 2, screen->height / 2 - 4 }, ColorWhite);
		}
		screen->type = GUIScreenTypeNone;
		GUIScreenRender(screen, mousePos);
		screen->type = GUIScreenTypeLoadLevel;
	}
	if (screen->type == GUIScreenTypeSaveLevel) {
		SaveLevelScreenRender(screen, mousePos);
	}
}

void LoadLevelScreenDestroy(LoadLevelScreen screen) {
	LoadLevelScreenData this = screen->typeData;
	MemoryFree(this);
}
