#include "OptionsScreen.h"
#include "ControlsScreen.h"
#include "Screen.h"
#include "../Minecraft.h"

OptionsScreen OptionsScreenCreate(GUIScreen parent, GameSettings settings)
{
	GUIScreen screen = GUIScreenCreate();
	screen->Type = GUIScreenTypeOptions;
	screen->TypeData = MemoryAllocate(sizeof(struct OptionsScreenData));
	OptionsScreenData this = screen->TypeData;
	this->Parent = parent;
	this->Settings = settings;
	this->Title = "Options";
	return screen;
}

void OptionsScreenOnOpen(OptionsScreen screen)
{
	OptionsScreenData this = screen->TypeData;
	for (int i = 0; i < this->Settings->SettingsCount; i++)
	{
		screen->Buttons = ListPush(screen->Buttons, &(Button){ ButtonCreateSize(i, screen->Width / 2 - 155 + i % 2 * 160, screen->Height / 6 + 24 * (i / 2 + 1) - 24, 150, 20, GameSettingsGetSetting(this->Settings, i)) });
	}
	
	screen->Buttons = ListPush(screen->Buttons, &(Button){ ButtonCreate(100, screen->Width / 2 - 100, screen->Height / 6 + 132, "Controls...") });
	screen->Buttons = ListPush(screen->Buttons, &(Button){ ButtonCreate(200, screen->Width / 2 - 100, screen->Height / 6 + 168, "Done") });
}

void OptionsScreenOnButtonClicked(OptionsScreen screen, Button button)
{
	OptionsScreenData this = screen->TypeData;
	if (button->Active)
	{
		if (button->ID < 100)
		{
			GameSettingsToggleSetting(this->Settings, button->ID, 1);
			button->Text = StringSet(button->Text, GameSettingsGetSetting(this->Settings, button->ID));
		}
		if (button->ID == 100) { MinecraftSetCurrentScreen(screen->Minecraft, ControlsScreenCreate(screen, this->Settings)); }
		if (button->ID == 200)
		{
			MinecraftSetCurrentScreen(screen->Minecraft, this->Parent);
		}
	}
}

void OptionsScreenRender(OptionsScreen screen, int2 mousePos)
{
	OptionsScreenData this = screen->TypeData;
	ScreenDrawFadingBox((int2){ 0, 0 }, (int2){ screen->Width, screen->Height }, ColorFromHex(0x05050060), ColorFromHex(0x303060A0));
	ScreenDrawCenteredString(screen->Font, this->Title, (int2){ screen->Width / 2, 20 }, ColorWhite);
}

void OptionsScreenDestroy(OptionsScreen screen)
{
	OptionsScreenData this = screen->TypeData;
	MemoryFree(this);
}
