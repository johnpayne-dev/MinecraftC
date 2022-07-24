#include "Timer.h"
#include "Utilities/Time.h"
#include <stdlib.h>

void TimerCreate(Timer * timer, float ticksPerSecond) {
	*timer = (Timer) {
		.speed = 1.0,
		.elapsedDelta = 0.0,
		.adjustment = 1.0,
		.ticksPerSecond = ticksPerSecond,
		.lastSystemClock = TimeMilli(),
		.lastHRClock = TimeNano() / 1e6,
	};
}
