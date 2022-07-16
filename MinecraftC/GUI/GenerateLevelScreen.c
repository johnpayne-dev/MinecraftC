#include "GenerateLevelScreen.h"
#include "Screen.h"
#include "../Minecraft.h"

GenerateLevelScreen GenerateLevelScreenCreate(GUIScreen parent) {
	GUIScreen screen = GUIScreenCreate();
	screen->type = GUIScreenTypeGenerateLevel;
	screen->typeData = MemoryAllocate(sizeof(struct GenerateLevelScreenData));
	GenerateLevelScreenData this = screen->typeData;
	this->parent = parent;
	return screen;
}

void GenerateLevelScreenOnOpen(GenerateLevelScreen screen) {
	for (int i = 0; i < ListCount(screen->buttons); i++) { ButtonDestroy(screen->buttons[i]); }
	screen->buttons = ListClear(screen->buttons);
	screen->buttons = ListPush(screen->buttons, &(Button){ ButtonCreate(0, screen->width / 2 - 100, screen->height / 4, "Small") });
	screen->buttons = ListPush(screen->buttons, &(Button){ ButtonCreate(1, screen->width / 2 - 100, screen->height / 4 + 24, "Normal") });
	screen->buttons = ListPush(screen->buttons, &(Button){ ButtonCreate(2, screen->width / 2 - 100, screen->height / 4 + 48, "Huge") });
	screen->buttons = ListPush(screen->buttons, &(Button){ ButtonCreate(3, screen->width / 2 - 100, screen->height / 4 + 120, "Cancel") });
}

void GenerateLevelScreenOnButtonClicked(GenerateLevelScreen screen, Button button) {
	GenerateLevelScreenData this = screen->typeData;
	if (button->id == 3) {
		MinecraftSetCurrentScreen(screen->minecraft, this->parent);
	} else {
		MinecraftGenerateLevel(screen->minecraft, button->id);
		MinecraftSetCurrentScreen(screen->minecraft, NULL);
		MinecraftGrabMouse(screen->minecraft);
	}
}

void GenerateLevelScreenRender(GenerateLevelScreen screen, int2 mousePos) {
	ScreenDrawFadingBox((int2){ 0, 0 }, (int2){ screen->width, screen->height }, ColorFromHex(0x05050060), ColorFromHex(0x303060A0));
	ScreenDrawCenteredString(screen->font, "Generate new level", (int2){ screen->width / 2, 40 }, ColorWhite);
}

void GenerateLevelScreenDestroy(GenerateLevelScreen screen) {
	GenerateLevelScreenData this = screen->typeData;
	MemoryFree(this);
}
