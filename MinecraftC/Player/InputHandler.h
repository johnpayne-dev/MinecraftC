#pragma once
#include <stdbool.h>
#include "../Utilities/LinearMath.h"
#include "../GameSettings.h"

typedef struct InputHandler
{
	bool keyStates[10];
	GameSettings settings;
	float2 xy;
	bool jumping;
} * InputHandler;

InputHandler InputHandlerCreate(GameSettings settings);
void InputHandlerSetKeyState(InputHandler input, int key, bool state);
void InputHandlerResetKeys(InputHandler input);
void InputHandlerUpdateMovement(InputHandler input);
void InputHandlerDestroy(InputHandler input);
