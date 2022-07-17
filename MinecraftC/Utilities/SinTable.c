#include "SinTable.h"
#include <math.h>

static float SinTable[0xFFFF];

void SinTableInitialize() {
	for (int i = 0; i < sizeof(SinTable) / sizeof(float); i++) {
		SinTable[i] = sin(i * 2.0 * M_PI / (float)0xFFFF);
	}
}

float tsin(float x) { return SinTable[(int)(x * 10430.378f) & 0xFFFF]; }
float tcos(float x) { return SinTable[(int)(x * 10430.378f + 16384.0f) & 0xFFFF]; }
