#include "ErrorScreen.h"
#include "Screen.h"

ErrorScreen ErrorScreenCreate(char * title, char * text)
{
	GUIScreen screen = GUIScreenCreate();
	screen->Type = GUIScreenTypeError;
	screen->TypeData = MemoryAllocate(sizeof(struct ErrorScreenData));
	ErrorScreenData this = screen->TypeData;
	this->Title = title;
	this->Text = text;
	return screen;
}

void ErrorScreenOnOpen(ErrorScreen screen)
{
	
}

void ErrorScreenRender(ErrorScreen screen, int2 mousePos)
{
	ErrorScreenData this = screen->TypeData;
	ScreenDrawFadingBox((int2){ 0, 0 }, (int2){ screen->Width, screen->Height }, ColorFromHex(0x402020FF), ColorFromHex(0x501010FF));
	ScreenDrawCenteredString(screen->Font, this->Title, (int2){ screen->Width / 2, 90 }, ColorWhite);
	ScreenDrawCenteredString(screen->Font, this->Text, (int2){ screen->Width / 2, 110 }, ColorWhite);
}

void ErrorScreenOnKeyPressed(ErrorScreen screen, char eventChar, int eventKey)
{
	
}

void ErrorScreenDestroy(ErrorScreen screen)
{
	ErrorScreenData this = screen->TypeData;
	MemoryFree(this);
}
