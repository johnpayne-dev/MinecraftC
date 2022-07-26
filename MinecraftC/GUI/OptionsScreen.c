#include "OptionsScreen.h"
#include "ControlsScreen.h"
#include "Screen.h"
#include "../Minecraft.h"

void OptionsScreenCreate(OptionsScreen * screen, GUIScreen * parent, GameSettings * settings) {
	GUIScreenCreate(screen);
	screen->type = GUIScreenTypeOptions;
	screen->options.parent = parent;
	screen->options.settings = settings;
	screen->options.title = "Options";
}

void OptionsScreenOnOpen(OptionsScreen * screen) {
	for (int i = 0; i < screen->options.settings->settingsCount; i++) {
		screen->buttons = ListPush(screen->buttons, &(Button){ 0 });
		ButtonCreateSize(&screen->buttons[i], i, screen->width / 2 - 155 + i % 2 * 160, screen->height / 6 + 24 * (i / 2 + 1) - 24, 150, 20, GameSettingsGetSetting(screen->options.settings, i));
	}
	
	screen->buttons = ListPush(screen->buttons, &(Button){ 0 });
	ButtonCreate(&screen->buttons[ListLength(screen->buttons) - 1], 100, screen->width / 2 - 100, screen->height / 6 + 132, "Controls...");
	screen->buttons = ListPush(screen->buttons, &(Button){ 0 });
	ButtonCreate(&screen->buttons[ListLength(screen->buttons) - 1], 200, screen->width / 2 - 100, screen->height / 6 + 168, "Done");
}

void OptionsScreenOnButtonClicked(OptionsScreen * screen, Button * button) {
	if (button->active) {
		if (button->id < 100) {
			GameSettingsToggleSetting(screen->options.settings, button->id);
			StringSet(&button->text, GameSettingsGetSetting(screen->options.settings, button->id));
		}
		if (button->id == 100) {
			ControlsScreen * controls = malloc(sizeof(ControlsScreen));
			ControlsScreenCreate(controls, screen, screen->options.settings);
			MinecraftSetCurrentScreen(screen->minecraft, controls);
		}
		if (button->id == 200) {
			MinecraftSetCurrentScreen(screen->minecraft, screen->options.parent);
		}
	}
}

void OptionsScreenRender(OptionsScreen * screen, int mx, int my) {
	ScreenDrawFadingBox(0, 0, screen->width, screen->height, 0x05050060, 0x303060A0);
	ScreenDrawCenteredString(screen->font, screen->options.title, screen->width / 2, 20, 0xffffffff);
}
