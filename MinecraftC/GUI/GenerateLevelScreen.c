#include "GenerateLevelScreen.h"
#include "Screen.h"
#include "../Minecraft.h"

void GenerateLevelScreenCreate(GenerateLevelScreen * screen, GUIScreen * parent) {
	GUIScreenCreate(screen);
	screen->type = GUIScreenTypeGenerateLevel;
	screen->generateLevel.parent = parent;
}

void GenerateLevelScreenOnOpen(GenerateLevelScreen * screen) {
	for (int i = 0; i < ListLength(screen->buttons); i++) { ButtonDestroy(&screen->buttons[i]); }
	screen->buttons = ListClear(screen->buttons);
	screen->buttons = ListPush(screen->buttons, &(Button){ 0 });
	ButtonCreate(&screen->buttons[0], 0, screen->width / 2 - 100, screen->height / 4, "Small");
	screen->buttons = ListPush(screen->buttons, &(Button){ 0 });
	ButtonCreate(&screen->buttons[1], 1, screen->width / 2 - 100, screen->height / 4 + 24, "Normal");
	screen->buttons = ListPush(screen->buttons, &(Button){ 0 });
	ButtonCreate(&screen->buttons[2], 2, screen->width / 2 - 100, screen->height / 4 + 48, "Huge");
	screen->buttons = ListPush(screen->buttons, &(Button){ 0 });
	ButtonCreate(&screen->buttons[3], 3, screen->width / 2 - 100, screen->height / 4 + 120, "Cancel");
}

void GenerateLevelScreenOnButtonClicked(GenerateLevelScreen * screen, Button * button) {
	if (button->id == 3) {
		MinecraftSetCurrentScreen(screen->minecraft, screen->generateLevel.parent);
	} else {
		LevelRegenerate(&screen->minecraft->level, button->id);
		MinecraftSetCurrentScreen(screen->minecraft, NULL);
		MinecraftGrabMouse(screen->minecraft);
	}
}

void GenerateLevelScreenRender(GenerateLevelScreen * screen, int mx, int my) {
	ScreenDrawFadingBox(0, 0, screen->width, screen->height, 0x05050060, 0x303060A0);
	ScreenDrawCenteredString(screen->font, "Generate new level", screen->width / 2, 40, 0xffffffff);
}
