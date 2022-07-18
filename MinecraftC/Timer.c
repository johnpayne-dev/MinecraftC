#include "Timer.h"
#include "Utilities/Time.h"
#include <stdlib.h>

Timer TimerCreate(float ticksPerSecond) {
	Timer timer = malloc(sizeof(struct Timer));
	*timer = (struct Timer) {
		.speed = 1.0,
		.elapsedDelta = 0.0,
		.adjustment = 1.0,
		.ticksPerSecond = ticksPerSecond,
		.lastSystemClock = TimeMilli(),
		.lastHRClock = TimeNano() / 1e6,
	};
	return timer;
}

void TimerDestroy(Timer timer) {
	free(timer);
}
