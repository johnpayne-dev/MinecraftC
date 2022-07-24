#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef char * String;

String StringCreate(const char * chars);

String StringCreateFromInt(int number);

int32_t StringLength(String string);

void StringConcat(String * string, const char * chars);

void StringConcatFront(const char * chars, String * string);

String StringSub(String string, int32_t indexStart, int32_t indexEnd);

int32_t StringIndexOf(String string, const char chr);

void StringSet(String * string, const char * chars);

void StringSetFromInt(String * string, int number);

void StringInsert(String * string, int32_t index, const char * chars);

void StringRemove(String * string, int32_t indexStart, int32_t indexEnd);

bool StringEquals(String string, const char * chars);

int StringToInt(String string);

void StringFree(String string);
