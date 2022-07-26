#if MINECRAFTC_MODS
#include "ModsScreen.h"
#include "Screen.h"
#include "../Minecraft.h"

void ModsScreenCreate(ModsScreen * screen, GUIScreen * parent, GameSettings * settings) {
	GUIScreenCreate(screen);
	screen->type = GUIScreenTypeMods;
	screen->mods.parent = parent;
	screen->mods.title = "Mods";
	screen->mods.settings = settings;
}

void ModsScreenOnOpen(ModsScreen * screen) {
	for (int i = 0; i < screen->mods.settings->modsCount; i++) {
		screen->buttons = ListPush(screen->buttons, &(Button){ 0 });
		String text = GameSettingsGetSetting(screen->mods.settings, screen->mods.settings->settingsCount + i);
		ButtonCreateSize(&screen->buttons[i], i, screen->width / 2 - 155 + i % 2 * 160, screen->height / 6 + 24 * (i / 2 + 1) - 24, 150, 20, text);
	}
	screen->buttons[1].active = false;
	Button button;
	ButtonCreate(&button, 200, screen->width / 2 - 100, screen->height / 6 + 168, "Done");
	screen->buttons = ListPush(screen->buttons, &button);
}

void ModsScreenRender(ModsScreen * screen, int mx, int my) {
	ScreenDrawFadingBox(0, 0, screen->width, screen->height, 0x05050060, 0x303060A0);
	ScreenDrawCenteredString(screen->font, screen->mods.title, screen->width / 2, 20, 0xFFFFFFFF);
}

void ModsScreenOnButtonClicked(ModsScreen * screen, Button * button) {
	if (button->id < 100 && button->active) {
		GameSettingsToggleSetting(screen->mods.settings, screen->mods.settings->settingsCount + button->id);
		StringSet(&button->text, GameSettingsGetSetting(screen->mods.settings, screen->mods.settings->settingsCount + button->id));
	}
	if (button->id == 200) {
		MinecraftSetCurrentScreen(screen->minecraft, screen->mods.parent);
	}
}

#endif
