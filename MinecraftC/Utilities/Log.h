#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

typedef enum LogType {
	LogTypeTrace,
	LogTypeDebug,
	LogTypeInfo,
	LogTypeWarning,
	LogTypeError,
	LogTypeFatal,
} LogType;

void LogSetFile(FILE * file);

void LogSetLevel(LogType level);

void LogSetQuiet(bool enable);

void LogUseColors(bool enable);

void Log(LogType level, const char * file, int line, const char * format, ...);

#define LogTrace(...) Log(LogTypeTrace, __FILE__, __LINE__, __VA_ARGS__)
#define LogDebug(...) Log(LogTypeDebug, __FILE__, __LINE__, __VA_ARGS__)
#define LogInfo(...) Log(LogTypeInfo,  __FILE__, __LINE__, __VA_ARGS__)
#define LogWarning(...) Log(LogTypeWarning,  __FILE__, __LINE__, __VA_ARGS__)
#define LogError(...) Log(LogTypeError, __FILE__, __LINE__, __VA_ARGS__)
#define LogFatal(...) Log(LogTypeFatal, __FILE__, __LINE__, __VA_ARGS__)
