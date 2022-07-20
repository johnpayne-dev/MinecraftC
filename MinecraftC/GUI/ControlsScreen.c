#include "ControlsScreen.h"
#include "Screen.h"
#include "../Minecraft.h"
#include "../Utilities/Log.h"

void ControlsScreenCreate(ControlsScreen * screen, GUIScreen * parent, GameSettings * settings) {
	GUIScreenCreate(screen);
	screen->type = GUIScreenTypeControls;
	screen->controls.parent = parent;
	screen->controls.selected = -1;
	screen->controls.settings = settings;
	screen->controls.title = "Controls";
}

void ControlsScreenOnOpen(ControlsScreen * screen) {
	for (int i = 0; i < ListLength(screen->controls.settings->bindings); i++) {
		String text = GameSettingsGetBinding(screen->controls.settings, i);
		Button button;
		ButtonCreateSize(&button, i, screen->width / 2 - 155 + i % 2 * 160, screen->height / 6 + 24 * (i / 2 + 1) - 24, 150, 20, text);
		screen->buttons = ListPush(screen->buttons, &button);
		StringFree(text);
	}
	Button button;
	ButtonCreate(&button, 200, screen->width / 2 - 100, screen->height / 6 + 168, "Done");
	screen->buttons = ListPush(screen->buttons, &button);
}

void ControlsScreenRender(ControlsScreen * screen, int mx, int my) {
	ScreenDrawFadingBox(0, 0, screen->width, screen->height, 0x05050060, 0x303060A0);
	ScreenDrawCenteredString(screen->font, screen->controls.title, screen->width / 2, 20, 0xFFFFFFFF);
}

void ControlsScreenOnKeyPressed(ControlsScreen * screen, char eventChar, int eventKey) {
	if (screen->controls.selected >= 0) {
		GameSettingsSetBinding(screen->controls.settings, screen->controls.selected, eventKey);
		String text = GameSettingsGetBinding(screen->controls.settings, screen->controls.selected);
		StringSet(&screen->buttons[screen->controls.selected].text, text);
		screen->controls.selected = -1;
		StringFree(text);
	} else {
		screen->type = GUIScreenTypeNone;
		GUIScreenOnKeyPressed(screen, eventChar, eventKey);
		screen->type = GUIScreenTypeControls;
	}
}

void ControlsScreenOnButtonClicked(ControlsScreen * screen, Button * button) {
	for (int i = 0; i < ListLength(screen->controls.settings->bindings); i++) {
		String text = GameSettingsGetBinding(screen->controls.settings, i);
		StringSet(&screen->buttons[i].text, text);
		StringFree(text);
	}
	
	if (button->id == 200) {
		MinecraftSetCurrentScreen(screen->minecraft, screen->controls.parent);
	} else {
		screen->controls.selected = button->id;
		String text = GameSettingsGetBinding(screen->controls.settings, button->id);
		StringConcatFront("> ", &text);
		StringConcat(&text, " <");
		StringSet(&button->text, text);
		StringFree(text);
	}
}
