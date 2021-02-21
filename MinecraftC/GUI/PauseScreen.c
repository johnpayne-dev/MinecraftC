#include "PauseScreen.h"
#include "OptionsScreen.h"
#include "GenerateLevelScreen.h"
#include "LoadLevelScreen.h"
#include "SaveLevelScreen.h"
#include "Screen.h"
#include "../Minecraft.h"

PauseScreen PauseScreenCreate()
{
	GUIScreen screen = GUIScreenCreate();
	screen->Type = GUIScreenTypePause;
	return screen;
}

void PauseScreenOnOpen(PauseScreen screen)
{
	for (int i = 0; i < ListCount(screen->Buttons); i++) { ButtonDestroy(screen->Buttons[i]); }
	screen->Buttons = ListClear(screen->Buttons);
	screen->Buttons = ListPush(screen->Buttons, &(Button){ ButtonCreate(0, screen->Width / 2 - 100, screen->Height / 4, "Options...") });
	screen->Buttons = ListPush(screen->Buttons, &(Button){ ButtonCreate(1, screen->Width / 2 - 100, screen->Height / 4 + 24, "Generate new level...") });
	screen->Buttons = ListPush(screen->Buttons, &(Button){ ButtonCreate(2, screen->Width / 2 - 100, screen->Height / 4 + 48, "Save level..") });
	screen->Buttons = ListPush(screen->Buttons, &(Button){ ButtonCreate(3, screen->Width / 2 - 100, screen->Height / 4 + 72, "Load level..") });
	screen->Buttons = ListPush(screen->Buttons, &(Button){ ButtonCreate(4, screen->Width / 2 - 100, screen->Height / 4 + 120, "Back to game") });
	
	screen->Buttons[1]->Active = true;
	screen->Buttons[2]->Active = false;
	screen->Buttons[3]->Active = false;
}

void PauseScreenOnButtonClicked(PauseScreen screen, Button button)
{
	if (button->ID == 0) { MinecraftSetCurrentScreen(screen->Minecraft, OptionsScreenCreate(screen, screen->Minecraft->Settings)); }
	if (button->ID == 1) { MinecraftSetCurrentScreen(screen->Minecraft, GenerateLevelScreenCreate(screen)); }
	if (screen->Minecraft->Session != NULL)
	{
		if (button->ID == 2) { MinecraftSetCurrentScreen(screen->Minecraft, SaveLevelScreenCreate(screen)); }
		if (button->ID == 3) { MinecraftSetCurrentScreen(screen->Minecraft, LoadLevelScreenCreate(screen)); }
	}
	if (button->ID == 4)
	{
		MinecraftGrabMouse(screen->Minecraft);
		MinecraftSetCurrentScreen(screen->Minecraft, NULL);
	}
}

void PauseScreenRender(PauseScreen screen, int2 mousePos)
{
	ScreenDrawFadingBox((int2){ 0, 0 }, (int2){ screen->Width, screen->Height }, ColorFromHex(0x05050060), ColorFromHex(0x303060A0));
	ScreenDrawCenteredString(screen->Font, "Game menu", (int2){ screen->Width / 2, 20 }, ColorWhite);
}
