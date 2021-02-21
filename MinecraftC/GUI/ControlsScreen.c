#include "ControlsScreen.h"
#include "Screen.h"
#include "../Minecraft.h"
#include "../Utilities/Log.h"

ControlsScreen ControlsScreenCreate(GUIScreen parent, GameSettings settings)
{
	GUIScreen screen = GUIScreenCreate();
	ControlsScreenData this = MemoryAllocate(sizeof(struct ControlsScreenData));
	screen->Type = GUIScreenTypeControls;
	screen->TypeData = this;
	this->Parent = parent;
	this->Selected = -1;
	this->Settings = settings;
	this->Title = "Controls";
	return screen;
}

void ControlsScreenOnOpen(ControlsScreen screen)
{
	ControlsScreenData this = screen->TypeData;
	for (int i = 0; i < ListCount(this->Settings->Bindings); i++)
	{
		String text = GameSettingsGetBinding(this->Settings, i);
		Button button = ButtonCreateSize(i, screen->Width / 2 - 155 + i % 2 * 160, screen->Height / 6 + 24 * (i / 2 + 1) - 24, 150, 20, text);
		screen->Buttons = ListPush(screen->Buttons, &button);
		StringDestroy(text);
	}
	Button button = ButtonCreate(200, screen->Width / 2 - 100, screen->Height / 6 + 168, "Done");
	screen->Buttons = ListPush(screen->Buttons, &button);
}

void ControlsScreenRender(ControlsScreen screen, int2 mousePos)
{
	ControlsScreenData this = screen->TypeData;
	ScreenDrawFadingBox((int2){ 0, 0 }, (int2){ screen->Width, screen->Height }, ColorFromHex(0x05050060), ColorFromHex(0x303060A0));
	ScreenDrawCenteredString(screen->Font, this->Title, (int2){ screen->Width / 2, 20 }, ColorWhite);
}

void ControlsScreenOnKeyPressed(ControlsScreen screen, char eventChar, int eventKey)
{
	ControlsScreenData this = screen->TypeData;
	if (this->Selected >= 0)
	{
		GameSettingsSetBinding(this->Settings, this->Selected, eventKey);
		String text = GameSettingsGetBinding(this->Settings, this->Selected);
		screen->Buttons[this->Selected]->Text = StringSet(screen->Buttons[this->Selected]->Text, text);
		this->Selected = -1;
		StringDestroy(text);
	}
	else
	{
		screen->Type = GUIScreenTypeNone;
		GUIScreenOnKeyPressed(screen, eventChar, eventKey);
		screen->Type = GUIScreenTypeControls;
	}
}

void ControlsScreenOnButtonClicked(ControlsScreen screen, Button button)
{
	ControlsScreenData this = screen->TypeData;
	for (int i = 0; i < ListCount(this->Settings->Bindings); i++)
	{
		String text = GameSettingsGetBinding(this->Settings, i);
		screen->Buttons[i]->Text = StringSet(screen->Buttons[i]->Text, text);
		StringDestroy(text);
	}
	
	if (button->ID == 200)
	{
		MinecraftSetCurrentScreen(screen->Minecraft, this->Parent);
	}
	else
	{
		this->Selected = button->ID;
		String text = StringConcat(StringConcatFront("> ", GameSettingsGetBinding(this->Settings, button->ID)), " <");
		button->Text = StringSet(button->Text, text);
		StringDestroy(text);
	}
}

void ControlsScreenDestroy(ControlsScreen screen)
{
	ControlsScreenData this = screen->TypeData;
	MemoryFree(this);
}
