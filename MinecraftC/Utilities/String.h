#pragma once
#include "List.h"

typedef list(char) String;

String StringCreate(char * chars);
String StringCreateFromInt(int number);
int StringLength(String string);
String StringConcat(String string, char * chars);
String StringConcatFront(char * chars, String string);
String StringSub(String string, int indexStart, int indexEnd);
int StringIndexOf(String string, char chr);
String StringSet(String string, char * chars);
String StringSetFromInt(String string, int number);
int StringToInt(String string);
void StringDestroy(String string);
