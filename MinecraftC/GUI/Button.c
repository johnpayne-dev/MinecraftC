#include "Button.h"

Button ButtonCreate(int buttonID, int x, int y, char * text)
{
	return ButtonCreateSize(buttonID, x, y, 200, 20, text);
}

Button ButtonCreateSize(int buttonID, int x, int y, int w, int h, char * text)
{
	Button button = MemoryAllocate(sizeof(struct Button));
	*button = (struct Button)
	{
		.ID = buttonID,
		.Position = (int2){ x, y },
		.Size = (int2){ w, 20 },
		.Text = StringCreate(text),
		.Visible = true,
		.Active = true,
	};
	return button;
}

void ButtonDestroy(Button button)
{
	StringDestroy(button->Text);
	MemoryFree(button);
}
