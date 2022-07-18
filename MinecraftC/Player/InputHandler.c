#include "InputHandler.h"

InputHandler InputHandlerCreate(GameSettings settings) {
	InputHandler input = malloc(sizeof(struct InputHandler));
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
	input->x = 0.0;
	input->y = 0.0;
	if (input->keyStates[0]) { input->y--; }
	if (input->keyStates[1]) { input->y++; }
	if (input->keyStates[2]) { input->x--; }
	if (input->keyStates[3]) { input->x++; }
	input->jumping = input->keyStates[4];
}

void InputHandlerDestroy(InputHandler input) {
	free(input);
}
