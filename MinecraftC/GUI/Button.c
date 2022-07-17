#include "Button.h"

Button ButtonCreate(int buttonID, int x, int y, char * text) {
	return ButtonCreateSize(buttonID, x, y, 200, 20, text);
}

Button ButtonCreateSize(int buttonID, int x, int y, int w, int h, char * text) {
	Button button = MemoryAllocate(sizeof(struct Button));
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
	return button;
}

void ButtonDestroy(Button button) {
	StringDestroy(button->text);
	MemoryFree(button);
}
