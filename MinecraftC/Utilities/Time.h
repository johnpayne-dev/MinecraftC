#pragma once
#include <stdint.h>
#include <time.h>
#ifdef _WIN32
	#include <Windows.h>
	#include <winsock.h>
#else
	#include <sys/time.h>
#endif

#ifdef _WIN32
static inline int gettimeofday(struct timeval * tp, void * tzp)
{
	FILETIME file_time;
	SYSSTEMTIME system_time;
	ULARGE_INTEGER ularge;
	GetSystemTime(&system_time);
	SystemTimeToFileTime(&system_time, &file_time);
	ularge.LowPart = file_time.dwLowDateTime;
	ularge.HighPart = file_time.dwHighDateTime;
	tp->tv_sec = (long)((ularge.QuadPart - ((unsigned __int64)116444736000000000ULL)) / 10000000L);
	tp->tv_usec = (long)(system_time.wMilliseconds * 1000);
	return 0;
}
#endif

static inline uint64_t TimeNano()
{
	struct timeval time;
	gettimeofday(&time, NULL);
	return 1000 * ((uint64_t)(time.tv_sec % 10000) * 1000000 + (uint64_t)time.tv_usec);
}

static inline uint64_t TimeMilli()
{
	return TimeNano() / 1000000;
}
