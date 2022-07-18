#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "String.h"

String StringCreate(const char * chars) {
	String string = malloc(strlen(chars) + 1);
	memcpy(string, chars, strlen(chars) + 1);
	return string;
}

String StringCreateFromInt(int n) {
	int len = n == 0 ? 1 : ceil(log10(abs(n) + 1.0));
	String string = StringCreate("");
	for (int i = 0; i < len; i++) {
		int digit = (int)((float)abs(n) / pow(10.0, i)) % 10;
		StringConcatFront((char[]){ digit + 48, '\0' }, &string);
	}
	if (n < 0) {
		StringConcatFront("-", &string);
	}
	return string;
}

int32_t StringLength(String string) {
	return (int32_t)strlen(string);
}

void StringConcat(String * string, const char * chars) {
	StringInsert(string, StringLength(*string), chars);
}

void StringConcatFront(const char * chars, String * string) {
	StringInsert(string, 0, chars);
}

String StringSub(String string, int32_t indexStart, int32_t indexEnd) {
	if (indexStart < 0) { indexStart = 0; }
	if (indexEnd > StringLength(string) - 1) { indexEnd = StringLength(string) - 1; }
	if (indexStart > indexEnd) { return StringCreate(""); }
	char orig = string[indexEnd + 1];
	string[indexEnd + 1] = '\0';
	String newString = StringCreate(string + indexStart);
	string[indexEnd + 1] = orig;
	return newString;
}

int32_t StringIndexOf(String string, char chr) {
	for (int32_t i = 0; i < StringLength(string); i++) {
		if (string[i] == chr) { return i; }
	}
	return -1;
}

void StringSet(String * string, const char * chars) {
	StringFree(*string);
	*string = StringCreate(chars);
}

void StringSetFromInt(String * string, int number) {
	StringFree(*string);
	*string = StringCreateFromInt(number);
}

void StringInsert(String * string, int32_t index, const char * chars) {
	int32_t len1 = StringLength(*string), len2 = (int32_t)strlen(chars);
	if (index > len1) { index = len1; }
	*string = realloc(*string, len1 + len2 + 1);
	for (int32_t i = len1; i >= index; i--) { (*string)[i + len2] = (*string)[i]; }
	memcpy(*string + index, chars, len2);
}

void StringRemove(String * string, int32_t indexStart, int32_t indexEnd) {
	if (indexStart < 0) { indexStart = 0; }
	if (indexEnd > StringLength(*string) - 1) { indexEnd = StringLength(*string) - 1; }
	if (indexStart > indexEnd) { StringSet(string, ""); }
	for (int i = 0; i < StringLength(*string) - indexEnd; i++) {
		(*string)[indexStart + i] = (*string)[indexEnd + i + 1];
	}
	*string = realloc(*string, StringLength(*string) - (indexEnd - indexStart + 1));
}

bool StringEquals(String string, const char * chars) {
	return strcmp(string, chars) == 0;
}

int StringToInt(String string) {
	int n = 0;
	for (int i = StringLength(string) - 1, j = 0; i >= 0; i--, j++) {
		if (string[i] == '-' && i == 0) { n *= -1; break; }
		n += (string[i] - 48) * pow(10, j);
	}
	return n;
}

void StringFree(String string) {
	free(string);
}
