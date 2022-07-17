#pragma once
#include <stdbool.h>
#include "../GameSettings.h"

typedef struct InputHandler {
	bool keyStates[10];
	GameSettings settings;
	float x, y;
	bool jumping;
} * InputHandler;

InputHandler InputHandlerCreate(GameSettings settings);
void InputHandlerSetKeyState(InputHandler input, int key, bool state);
void InputHandlerResetKeys(InputHandler input);
void InputHandlerUpdateMovement(InputHandler input);
void InputHandlerDestroy(InputHandler input);
