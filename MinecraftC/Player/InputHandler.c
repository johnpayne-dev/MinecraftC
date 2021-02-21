#include "InputHandler.h"

InputHandler InputHandlerCreate(GameSettings settings)
{
	InputHandler input = MemoryAllocate(sizeof(struct InputHandler));
	*input = (struct InputHandler)
	{
		.Settings = settings,
	};
	return input;
}

void InputHandlerSetKeyState(InputHandler input, int key, bool state)
{
	if (key == input->Settings->ForwardKey.Key) { input->KeyStates[0] = state; }
	if (key == input->Settings->BackKey.Key) { input->KeyStates[1] = state; }
	if (key == input->Settings->LeftKey.Key) { input->KeyStates[2] = state; }
	if (key == input->Settings->RightKey.Key) { input->KeyStates[3] = state; }
	if (key == input->Settings->JumpKey.Key) { input->KeyStates[4] = state; }
}

void InputHandlerResetKeys(InputHandler input)
{
	for (int i = 0; i < 10; i++) { input->KeyStates[i] = false; }
}

void InputHandlerUpdateMovement(InputHandler input)
{
	input->XY = zero2f;
	if (input->KeyStates[0]) { input->XY.y--; }
	if (input->KeyStates[1]) { input->XY.y++; }
	if (input->KeyStates[2]) { input->XY.x--; }
	if (input->KeyStates[3]) { input->XY.x++; }
	input->Jumping = input->KeyStates[4];
}

void InputHandlerDestroy(InputHandler input)
{
	MemoryFree(input);
}
