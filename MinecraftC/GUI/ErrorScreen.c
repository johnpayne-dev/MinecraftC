#include "ErrorScreen.h"
#include "Screen.h"

void ErrorScreenCreate(ErrorScreen * screen, char * title, char * text) {
	GUIScreenCreate(screen);
	screen->type = GUIScreenTypeError;
	screen->error.title = title;
	screen->error.text = text;
}

void ErrorScreenOnOpen(ErrorScreen * screen) {
}

void ErrorScreenRender(ErrorScreen * screen, int mx, int my) {
	ScreenDrawFadingBox(0, 0, screen->width, screen->height, 0x402020FF, 0x501010FF);
	ScreenDrawCenteredString(screen->font, screen->error.title, screen->width / 2, 90, 0xffffffff);
	ScreenDrawCenteredString(screen->font, screen->error.text, screen->width / 2, 110, 0xffffffff);
}

void ErrorScreenOnKeyPressed(ErrorScreen * screen, char eventChar, int eventKey) {
}
