#include "GenerateLevelScreen.h"
#include "Screen.h"
#include "../Minecraft.h"

GenerateLevelScreen GenerateLevelScreenCreate(GUIScreen parent)
{
	GUIScreen screen = GUIScreenCreate();
	screen->Type = GUIScreenTypeGenerateLevel;
	screen->TypeData = MemoryAllocate(sizeof(struct GenerateLevelScreenData));
	GenerateLevelScreenData this = screen->TypeData;
	this->Parent = parent;
	return screen;
}

void GenerateLevelScreenOnOpen(GenerateLevelScreen screen)
{
	for (int i = 0; i < ListCount(screen->Buttons); i++) { ButtonDestroy(screen->Buttons[i]); }
	screen->Buttons = ListClear(screen->Buttons);
	screen->Buttons = ListPush(screen->Buttons, &(Button){ ButtonCreate(0, screen->Width / 2 - 100, screen->Height / 4, "Small") });
	screen->Buttons = ListPush(screen->Buttons, &(Button){ ButtonCreate(1, screen->Width / 2 - 100, screen->Height / 4 + 24, "Normal") });
	screen->Buttons = ListPush(screen->Buttons, &(Button){ ButtonCreate(2, screen->Width / 2 - 100, screen->Height / 4 + 48, "Huge") });
	screen->Buttons = ListPush(screen->Buttons, &(Button){ ButtonCreate(3, screen->Width / 2 - 100, screen->Height / 4 + 120, "Cancel") });
}

void GenerateLevelScreenOnButtonClicked(GenerateLevelScreen screen, Button button)
{
	GenerateLevelScreenData this = screen->TypeData;
	if (button->ID == 3)
	{
		MinecraftSetCurrentScreen(screen->Minecraft, this->Parent);
	}
	else
	{
		MinecraftGenerateLevel(screen->Minecraft, button->ID);
		MinecraftSetCurrentScreen(screen->Minecraft, NULL);
		MinecraftGrabMouse(screen->Minecraft);
	}
}

void GenerateLevelScreenRender(GenerateLevelScreen screen, int2 mousePos)
{
	ScreenDrawFadingBox((int2){ 0, 0 }, (int2){ screen->Width, screen->Height }, ColorFromHex(0x05050060), ColorFromHex(0x303060A0));
	ScreenDrawCenteredString(screen->Font, "Generate new level", (int2){ screen->Width / 2, 40 }, ColorWhite);
}

void GenerateLevelScreenDestroy(GenerateLevelScreen screen)
{
	GenerateLevelScreenData this = screen->TypeData;
	MemoryFree(this);
}
