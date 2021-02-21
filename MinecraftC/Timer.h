#pragma once
#include <stdint.h>

typedef struct Timer
{
	float TicksPerSecond;
	float LastHR;
	int ElapsedTicks;
	float Delta;
	float Speed;
	float ElapsedDelta;
	uint64_t LastSystemClock;
	uint64_t LastHRClock;
	float Adjustment;
} * Timer;

Timer TimerCreate(float ticksPerSecond);
void TimerDestroy(Timer timer);
