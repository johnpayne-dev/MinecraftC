#include "Button.h"
#include <stdlib.h>

void ButtonCreate(Button * button, int buttonID, int x, int y, char * text) {
	ButtonCreateSize(button, buttonID, x, y, 200, 20, text);
}

void ButtonCreateSize(Button * button, int buttonID, int x, int y, int w, int h, char * text) {
	*button = (struct Button) {
		.id = buttonID,
		.x = x,
		.y = y,
		.width = w,
		.height = 20,
		.text = StringCreate(text),
		.visible = true,
		.active = true,
	};
}

void ButtonDestroy(Button * button) {
	StringFree(button->text);
}
