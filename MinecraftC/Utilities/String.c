#include <string.h>
#include <math.h>
#include "String.h"

String StringCreate(char * str) {
	String string = ListCreate(sizeof(char));
	int i = 0;
	while (str[i] != '\0') { string = ListPush(string, &str[i]); i++; }
	string = ListPush(string, &(char){ '\0' });
	return string;
}

String StringCreateFromInt(int n) {
	int len = n == 0 ? 1 : ceil(log10(abs(n) + 1.0));
	String string = StringCreate("");
	for (int i = 0; i < len; i++) {
		int digit = (int)((float)abs(n) / pow(10.0, i)) % 10;
		string = StringConcatFront((char[]){ digit + 48, '\0' }, string);
	}
	return n < 0 ? StringConcatFront("-", string) : string;
}

int StringLength(String string) {
	return ListCount(string) - 1;
}

String StringConcat(String string, char * str) {
	char * copy = MemoryAllocate(strlen(str) + 1);
	strcpy(copy, str);
	string = ListPop(string);
	int i = 0;
	while (copy[i] != '\0') { string = ListPush(string, &copy[i]); i++; }
	string = ListPush(string, &(char){ '\0' });
	MemoryFree(copy);
	return string;
}

String StringConcatFront(char * str, String string) {
	char * copy = MemoryAllocate(strlen(str) + 1);
	strcpy(copy, str);
	int i = 0;
	while (copy[i] != '\0') { string = ListInsert(string, &copy[i], i); i++; }
	MemoryFree(copy);
	return string;
}

String StringSub(String string, int indexStart, int indexEnd) {
	string = ListPop(string);
	for (int i = ListCount(string); i > indexEnd; i--) { string = ListPop(string); }
	for (int i = 0; i < indexStart; i++) { string = ListRemove(string, 0); }
	string = ListPush(string, &(char){ '\0' });
	return string;
}

int StringIndexOf(String string, char chr) {
	for (int i = 0; i < StringLength(string); i++) {
		if (string[i] == chr) { return i; }
	}
	return -1;
}

String StringSet(String string, char * chars) {
	char * copy = MemoryAllocate(strlen(chars) + 1);
	strcpy(copy, chars);
	string = StringSub(string, 0, 0);
	string = StringConcat(string, copy);
	MemoryFree(copy);
	return string;
}

String StringSetFromInt(String string, int number) {
	StringDestroy(string);
	return StringCreateFromInt(number);
}

int StringToInt(String string) {
	int n = 0;
	for (int i = StringLength(string) - 1, j = 0; i >= 0; i--, j++) {
		if (string[i] == '-' && i == 0) { n *= -1; break; }
		n += (string[i] - 48) * pow(10, j);
	}
	return n;
}

void StringDestroy(String string) {
	ListDestroy(string);
}
