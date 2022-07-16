#include "InputHandler.h"

InputHandler InputHandlerCreate(GameSettings settings) {
	InputHandler input = MemoryAllocate(sizeof(struct InputHandler));
	*input = (struct InputHandler) {
		.settings = settings,
	};
	return input;
}

void InputHandlerSetKeyState(InputHandler input, int key, bool state) {
	if (key == input->settings->forwardKey.key) { input->keyStates[0] = state; }
	if (key == input->settings->backKey.key) { input->keyStates[1] = state; }
	if (key == input->settings->leftKey.key) { input->keyStates[2] = state; }
	if (key == input->settings->rightKey.key) { input->keyStates[3] = state; }
	if (key == input->settings->jumpKey.key) { input->keyStates[4] = state; }
}

void InputHandlerResetKeys(InputHandler input) {
	for (int i = 0; i < 10; i++) { input->keyStates[i] = false; }
}

void InputHandlerUpdateMovement(InputHandler input) {
	input->xy = zero2f;
	if (input->keyStates[0]) { input->xy.y--; }
	if (input->keyStates[1]) { input->xy.y++; }
	if (input->keyStates[2]) { input->xy.x--; }
	if (input->keyStates[3]) { input->xy.x++; }
	input->jumping = input->keyStates[4];
}

void InputHandlerDestroy(InputHandler input) {
	MemoryFree(input);
}
