#include <string.h>
#include "LevelNameScreen.h"
#include "Screen.h"
#include "../Minecraft.h"

LevelNameScreen LevelNameScreenCreate(GUIScreen parent, char * name, int id)
{
	GUIScreen screen = GUIScreenCreate();
	screen->Type = GUIScreenTypeLevelName;
	screen->TypeData = MemoryAllocate(sizeof(struct LevelNameScreenData));
	LevelNameScreenData this = screen->TypeData;
	this->Title = "Enter level name:";
	this->Counter = 0;
	this->Parent = parent;
	this->Name = StringCreate(name);
	if (strcmp(name, "-") == 0) { this->Name = StringSet(this->Name, ""); }
	return screen;
}

void LevelNameScreenOnOpen(LevelNameScreen screen)
{
	LevelNameScreenData this = screen->TypeData;
	for (int i = 0; i < ListCount(screen->Buttons); i++) { ButtonDestroy(screen->Buttons[i]); }
	screen->Buttons = ListClear(screen->Buttons);
	//Keyboard.enableRepeatEvents(true);
	screen->Buttons = ListPush(screen->Buttons, &(Button){ ButtonCreate(0, screen->Width / 2 - 100, screen->Height / 4 + 120, "Save") });
	screen->Buttons = ListPush(screen->Buttons, &(Button){ ButtonCreate(0, screen->Width / 2 - 100, screen->Height / 4 + 144, "Cancel") });
	screen->Buttons[0]->Active = StringLength(this->Name) > 0;
}

void LevelNameScreenOnClose(LevelNameScreen screen)
{
	//Keyboard.enableRepeatEvents(false);
}

void LevelNameScreenTick(LevelNameScreen screen)
{
	LevelNameScreenData this = screen->TypeData;
	this->Counter++;
}

void LevelNameScreenRender(LevelNameScreen screen, int2 mousePos)
{
	LevelNameScreenData this = screen->TypeData;
	ScreenDrawFadingBox((int2){ 0, 0 }, (int2){ screen->Width, screen->Height }, ColorFromHex(0x05050060), ColorFromHex(0x303060A0));
	ScreenDrawCenteredString(screen->Font, this->Title, (int2){ screen->Width / 2, 40 }, ColorWhite);
	int x = screen->Width / 2 - 100;
	int y = screen->Height / 2 - 10;
	ScreenDrawBox((int2){ x - 1, y - 1 }, (int2){ x + 201, y + 21 }, ColorFromHex(0xA0A0A0FF));
	ScreenDrawBox((int2){ x, y }, (int2){ x + 200, y + 20 }, ColorBlack);
	String string = StringConcat(StringCreate(this->Name), this->Counter / 6 % 2 == 0 ? "_" : "");
	ScreenDrawString(screen->Font, string, (int2){ x + 4, y + 6 }, ColorFromHex(0xE0E0E0FF));
	StringDestroy(string);
}

void LevelNameScreenOnKeyPressed(LevelNameScreen screen, char eventChar, int eventKey)
{
	LevelNameScreenData this = screen->TypeData;
	if (eventKey == 14 && StringLength(this->Name) > 0) { this->Name = StringSub(this->Name, 0, StringLength(this->Name) - 1); }
	
	String allowedChars = StringCreate("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ,.:-_\'*!\"#%/()=+?[]{}<>");
	if (StringIndexOf(allowedChars, eventChar) >= 0 && StringLength(this->Name) < 64)
	{
		this->Name = StringConcat(this->Name, (char[]){ eventChar, '\0' });
	}
	
	screen->Buttons[0]->Active = StringLength(this->Name) > 0;
}

void LevelNameScreenOnButtonClicked(LevelNameScreen screen, Button button)
{
	LevelNameScreenData this = screen->TypeData;
	if (button->Active)
	{
		if (button->ID == 0 && StringLength(this->Name) > 0)
		{
			LevelIOSaveOnline(screen->Minecraft->LevelIO, screen->Minecraft->Level, screen->Minecraft->Host, screen->Minecraft->Session->UserName, screen->Minecraft->Session->SessionID, this->Name, this->ID);
			MinecraftSetCurrentScreen(screen->Minecraft, NULL);
			MinecraftGrabMouse(screen->Minecraft);
		}
		
		if (button->ID == 1)
		{
			MinecraftSetCurrentScreen(screen->Minecraft, this->Parent);
		}
	}
}

void LevelNameScreenDestroy(LevelNameScreen screen)
{
	LevelNameScreenData this = screen->TypeData;
	MemoryFree(this);
}
