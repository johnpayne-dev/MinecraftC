#include "OptionsScreen.h"
#include "ControlsScreen.h"
#include "Screen.h"
#include "../Minecraft.h"

OptionsScreen OptionsScreenCreate(GUIScreen parent, GameSettings settings) {
	GUIScreen screen = GUIScreenCreate();
	screen->type = GUIScreenTypeOptions;
	screen->typeData = MemoryAllocate(sizeof(struct OptionsScreenData));
	OptionsScreenData this = screen->typeData;
	this->parent = parent;
	this->settings = settings;
	this->title = "Options";
	return screen;
}

void OptionsScreenOnOpen(OptionsScreen screen) {
	OptionsScreenData this = screen->typeData;
	for (int i = 0; i < ListCount(screen->buttons); i++) { ButtonDestroy(screen->buttons[i]); }
	screen->buttons = ListClear(screen->buttons);
	for (int i = 0; i < this->settings->settingsCount; i++) {
		screen->buttons = ListPush(screen->buttons, &(Button){ ButtonCreateSize(i, screen->width / 2 - 155 + i % 2 * 160, screen->height / 6 + 24 * (i / 2 + 1) - 24, 150, 20, GameSettingsGetSetting(this->settings, i)) });
	}
	
	screen->buttons = ListPush(screen->buttons, &(Button){ ButtonCreate(100, screen->width / 2 - 100, screen->height / 6 + 132, "Controls...") });
	screen->buttons = ListPush(screen->buttons, &(Button){ ButtonCreate(200, screen->width / 2 - 100, screen->height / 6 + 168, "Done") });
}

void OptionsScreenOnButtonClicked(OptionsScreen screen, Button button) {
	OptionsScreenData this = screen->typeData;
	if (button->active) {
		if (button->id < 100) {
			GameSettingsToggleSetting(this->settings, button->id, 1);
			button->text = StringSet(button->text, GameSettingsGetSetting(this->settings, button->id));
		}
		if (button->id == 100) { MinecraftSetCurrentScreen(screen->minecraft, ControlsScreenCreate(screen, this->settings)); }
		if (button->id == 200) {
			MinecraftSetCurrentScreen(screen->minecraft, this->parent);
		}
	}
}

void OptionsScreenRender(OptionsScreen screen, int2 mousePos) {
	OptionsScreenData this = screen->typeData;
	ScreenDrawFadingBox((int2){ 0, 0 }, (int2){ screen->width, screen->height }, ColorFromHex(0x05050060), ColorFromHex(0x303060A0));
	ScreenDrawCenteredString(screen->font, this->title, (int2){ screen->width / 2, 20 }, ColorWhite);
}

void OptionsScreenDestroy(OptionsScreen screen) {
	OptionsScreenData this = screen->typeData;
	MemoryFree(this);
}
