#include "ErrorScreen.h"
#include "Screen.h"

ErrorScreen ErrorScreenCreate(char * title, char * text) {
	GUIScreen screen = GUIScreenCreate();
	screen->type = GUIScreenTypeError;
	screen->typeData = MemoryAllocate(sizeof(struct ErrorScreenData));
	ErrorScreenData this = screen->typeData;
	this->title = title;
	this->text = text;
	return screen;
}

void ErrorScreenOnOpen(ErrorScreen screen) {
}

void ErrorScreenRender(ErrorScreen screen, int2 mousePos) {
	ErrorScreenData this = screen->typeData;
	ScreenDrawFadingBox((int2){ 0, 0 }, (int2){ screen->width, screen->height }, ColorFromHex(0x402020FF), ColorFromHex(0x501010FF));
	ScreenDrawCenteredString(screen->font, this->title, (int2){ screen->width / 2, 90 }, ColorWhite);
	ScreenDrawCenteredString(screen->font, this->text, (int2){ screen->width / 2, 110 }, ColorWhite);
}

void ErrorScreenOnKeyPressed(ErrorScreen screen, char eventChar, int eventKey) {
}

void ErrorScreenDestroy(ErrorScreen screen) {
	ErrorScreenData this = screen->typeData;
	MemoryFree(this);
}
