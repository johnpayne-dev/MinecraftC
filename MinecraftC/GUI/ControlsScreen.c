#include "ControlsScreen.h"
#include "Screen.h"
#include "../Minecraft.h"
#include "../Utilities/Log.h"

ControlsScreen ControlsScreenCreate(GUIScreen parent, GameSettings * settings) {
	GUIScreen screen = GUIScreenCreate();
	ControlsScreenData this = malloc(sizeof(struct ControlsScreenData));
	screen->type = GUIScreenTypeControls;
	screen->typeData = this;
	this->parent = parent;
	this->selected = -1;
	this->settings = settings;
	this->title = "Controls";
	return screen;
}

void ControlsScreenOnOpen(ControlsScreen screen) {
	ControlsScreenData this = screen->typeData;
	for (int i = 0; i < ListLength(this->settings->bindings); i++)
	{
		String text = GameSettingsGetBinding(this->settings, i);
		Button button = ButtonCreateSize(i, screen->width / 2 - 155 + i % 2 * 160, screen->height / 6 + 24 * (i / 2 + 1) - 24, 150, 20, text);
		screen->buttons = ListPush(screen->buttons, &button);
		StringFree(text);
	}
	Button button = ButtonCreate(200, screen->width / 2 - 100, screen->height / 6 + 168, "Done");
	screen->buttons = ListPush(screen->buttons, &button);
}

void ControlsScreenRender(ControlsScreen screen, int mx, int my) {
	ControlsScreenData this = screen->typeData;
	ScreenDrawFadingBox(0, 0, screen->width, screen->height, 0x05050060, 0x303060A0);
	ScreenDrawCenteredString(screen->font, this->title, screen->width / 2, 20, 0xFFFFFFFF);
}

void ControlsScreenOnKeyPressed(ControlsScreen screen, char eventChar, int eventKey) {
	ControlsScreenData this = screen->typeData;
	if (this->selected >= 0) {
		GameSettingsSetBinding(this->settings, this->selected, eventKey);
		String text = GameSettingsGetBinding(this->settings, this->selected);
		StringSet(&screen->buttons[this->selected]->text, text);
		this->selected = -1;
		StringFree(text);
	} else {
		screen->type = GUIScreenTypeNone;
		GUIScreenOnKeyPressed(screen, eventChar, eventKey);
		screen->type = GUIScreenTypeControls;
	}
}

void ControlsScreenOnButtonClicked(ControlsScreen screen, Button button) {
	ControlsScreenData this = screen->typeData;
	for (int i = 0; i < ListLength(this->settings->bindings); i++) {
		String text = GameSettingsGetBinding(this->settings, i);
		StringSet(&screen->buttons[i]->text, text);
		StringFree(text);
	}
	
	if (button->id == 200) {
		MinecraftSetCurrentScreen(screen->minecraft, this->parent);
	} else {
		this->selected = button->id;
		String text = GameSettingsGetBinding(this->settings, button->id);
		StringConcatFront("> ", &text);
		StringConcat(&text, " <");
		StringSet(&button->text, text);
		StringFree(text);
	}
}

void ControlsScreenDestroy(ControlsScreen screen) {
	ControlsScreenData this = screen->typeData;
	free(this);
}
