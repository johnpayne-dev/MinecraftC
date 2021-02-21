#pragma once
#include <stdlib.h>
#include "Memory.h"

#define list(type) type*

list(void) ListCreate(unsigned int elementSize);

unsigned int ListCount(list(void) list);

unsigned int ListElementSize(list(void) list);

unsigned int ListCapacity(list(void) list);

list(void) ListInsert(list(void) list, void * value, int index);

list(void) ListRemove(list(void) list, int index);

list(void) ListPush(list(void) list, void * value);

list(void) ListPop(list(void) list);

list(void) ListRemoveAll(list(void) list, void * value);

list(void) ListRemoveFirst(list(void) list, void * value);

list(void) ListRemoveLast(list(void) list, void * value);

_Bool ListContains(list(void) list, void * value);

list(void) ListClear(list(void) list);

void ListDestroy(list(void) list);
