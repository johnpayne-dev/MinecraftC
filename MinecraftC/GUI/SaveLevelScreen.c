#include "SaveLevelScreen.h"
#include "LevelNameScreen.h"
#include "Screen.h"
#include "../Minecraft.h"

SaveLevelScreen SaveLevelScreenCreate(GUIScreen parent)
{
	GUIScreen screen = LoadLevelScreenCreate(parent);
	screen->Type = GUIScreenTypeSaveLevel;
	LoadLevelScreenData this = screen->TypeData;
	this->Title = "Save level";
	this->Saving = true;
	this->Parent = parent;
	return screen;
}

void SaveLevelScreenSetLevels(SaveLevelScreen screen, char * strings[5])
{
	for (int i = 0; i < 5; i++)
	{
		screen->Buttons[i]->Text = StringSet(screen->Buttons[i]->Text, strings[i]);
		screen->Buttons[i]->Visible = true;
		screen->Buttons[i]->Active = screen->Minecraft->Session->HasPaid;
	}
}

void SaveLevelScreenOnOpen(SaveLevelScreen screen)
{
	screen->Buttons[5]->Text = StringSet(screen->Buttons[5]->Text, "Save file...");
}

void SaveLevelScreenOpenLevel(SaveLevelScreen screen, int level)
{
	MinecraftSetCurrentScreen(screen->Minecraft, LevelNameScreenCreate(screen, screen->Buttons[level]->Text, level));
}

void SaveLevelScreenOpenLevelFromFile(SaveLevelScreen screen, char * file)
{
	LoadLevelScreenData this = screen->TypeData;
	LevelIOSave(screen->Minecraft->LevelIO, screen->Minecraft->Level, SDL_RWFromFile(file, "wb"));
	MinecraftSetCurrentScreen(screen->Minecraft, this->Parent);
}

void SaveLevelScreenRender(SaveLevelScreen screen, int2 mousePos)
{
	if (!screen->Minecraft->Session->HasPaid)
	{
		ScreenDrawFadingBox((int2){ screen->Width / 2 - 80, 72 }, (int2){ screen->Width / 2 + 80, 120 }, ColorFromHex(0x000000E0), ColorFromHex(0x000000E0));
		ScreenDrawCenteredString(screen->Font, "Premium only!", (int2){ screen->Width / 2, 80 }, ColorFromHex(0xFF9090FF));
		ScreenDrawCenteredString(screen->Font, "Purchase the game to be able", (int2){ screen->Width / 2, 96 }, ColorFromHex(0xE08080FF));
		ScreenDrawCenteredString(screen->Font, "to save your levels online.", (int2){ screen->Width / 2, 104 }, ColorFromHex(0xE08080FF));
	}
}
