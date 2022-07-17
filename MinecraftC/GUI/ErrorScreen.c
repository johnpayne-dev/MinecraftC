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

void ErrorScreenRender(ErrorScreen screen, int mx, int my) {
	ErrorScreenData this = screen->typeData;
	ScreenDrawFadingBox(0, 0, screen->width, screen->height, 0x402020FF, 0x501010FF);
	ScreenDrawCenteredString(screen->font, this->title, screen->width / 2, 90, 0xffffffff);
	ScreenDrawCenteredString(screen->font, this->text, screen->width / 2, 110, 0xffffffff);
}

void ErrorScreenOnKeyPressed(ErrorScreen screen, char eventChar, int eventKey) {
}

void ErrorScreenDestroy(ErrorScreen screen) {
	ErrorScreenData this = screen->typeData;
	MemoryFree(this);
}
