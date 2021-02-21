#include "Timer.h"
#include "Utilities/Memory.h"
#include "Utilities/Time.h"

Timer TimerCreate(float ticksPerSecond)
{
	Timer timer = MemoryAllocate(sizeof(struct Timer));
	*timer = (struct Timer)
	{
		.Speed = 1.0,
		.ElapsedDelta = 0.0,
		.Adjustment = 1.0,
		.TicksPerSecond = ticksPerSecond,
		.LastSystemClock = TimeMilli(),
		.LastHRClock = TimeNano() / 1e6,
	};
	return timer;
}

void TimerDestroy(Timer timer)
{
	MemoryFree(timer);
}
