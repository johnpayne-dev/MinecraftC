#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include "Log.h"

static struct
{
	FILE * File;
	LogType Level;
	bool Quiet;
	bool UseColors;
} LogState = { 0 };

static const char * LevelNames[] = { "TRACE", "DEBUG", "INFO", "WARNING", "ERROR", "FATAL" };

static const char * LevelColors[] = { "\x1b[94m", "\x1b[36m", "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[35m" };

void LogSetFile(FILE * file) { LogState.File = file; }

void LogSetLevel(LogType level) { LogState.Level = level; }

void LogSetQuiet(bool enable) { LogState.Quiet = enable; }

void LogUseColors(bool enable) { LogState.UseColors = enable; }

void Log(LogType level, const char * file, int line, const char * format, ...)
{
	if (level < LogState.Level) { return; }

	time_t t = time(NULL);
#ifdef _WIN32
	struct tm l;
	struct tm * lt = &l;
	localtime_s(lt, &t);
#else
	struct tm *lt = localtime(&t);
#endif

	if (!LogState.Quiet)
	{
		va_list args;
		char buf[16];
		buf[strftime(buf, sizeof(buf), "%H:%M:%S", lt)] = '\0';
		if (LogState.UseColors) { fprintf(stderr, "%s \x1b[90m%s:%d:\x1b[0m %s%-5s\x1b[0m", buf, file, line, LevelColors[level], LevelNames[level]); }
		else { fprintf(stderr, "%s %s:%d %-5s: ", buf, file, line, LevelNames[level]); }
		
		va_start(args, format);
		vfprintf(stderr, format, args);
		va_end(args);
		fprintf(stderr, "");
		fflush(stderr);
	}

	if (LogState.File != NULL)
	{
		va_list args;
		char buf[32];
		buf[strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", lt)] = '\0';
		fprintf(LogState.File, "%s %-5s %s:%d: ", buf, LevelNames[level], file, line);
		va_start(args, format);
		vfprintf(LogState.File, format, args);
		va_end(args);
		fprintf(LogState.File, "\n");
		fflush(LogState.File);
	}
	
	if (level == LogTypeFatal) { abort(); }
}
