#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "List.h"

static const unsigned int MetaSize = 2 * sizeof(unsigned int);

list(void) ListCreate(unsigned int elementSize) {
	unsigned int * list = malloc(MetaSize + 2 * elementSize);
	list[0] = elementSize;
	list[1] = 0;
	return (uint8_t *)list + MetaSize;
}

struct ListData {
	unsigned int elementSize;
	unsigned int * count;
	unsigned int capacity;
};

static struct ListData ListData(list(void) list) {
	struct ListData data = { 0 };
	data.elementSize = *(unsigned int *)((uint8_t *)list - MetaSize);
	data.count = (unsigned int *)((uint8_t *)list - MetaSize / 2);
	data.capacity = pow(2.0, floor(log2(*data.count)) + 1.0);
	return data;
}

unsigned int ListCount(list(void) list) {
	return *(unsigned int *)((uint8_t *)list - MetaSize / 2);
}

unsigned int ListElementSize(list(void) list) {
	return *(unsigned int *)((uint8_t *)list - MetaSize);
}

unsigned int ListCapacity(list(void) list) {
	return ListData(list).capacity;
}

list(void) ListInsert(list(void) list, void * element, int index) {
	struct ListData data = ListData(list);
	if (index < 0 || index > *data.count) {
		//printf("Trying to add a value to a list, but the index is out of bounds.\n");
		exit(1);
	}
	
	(*data.count)++;
	if (*data.count == data.capacity) {
		list = (uint8_t *)realloc((uint8_t *)list - MetaSize, MetaSize + 2 * data.capacity * data.elementSize) + MetaSize;
	}
	data = ListData(list);
	memmove((uint8_t *)list + (index + 1) * data.elementSize, (uint8_t *)list + index * data.elementSize, (*data.count - 1 - index) * data.elementSize);
	memcpy((uint8_t *)list + index * data.elementSize, element, data.elementSize);
	return list;
}

list(void) ListRemove(list(void) list, int index) {
	struct ListData data = ListData(list);
	if (index < 0 || index > *data.count) {
		//printf("Trying to remove a value from a list, but the index is out of bounds.\n");
		exit(1);
	}

	for (int j = (index + 1) * (int)data.elementSize; j < (*data.count) * (int)data.elementSize; j++) {
		((uint8_t *)list)[j - data.elementSize] = ((uint8_t *)list)[j];
	}
	(*data.count)--;
	if (*data.count == data.capacity / 2 - 1) {
		list = (uint8_t *)realloc((uint8_t *)list - MetaSize, MetaSize + (data.capacity / 2) * data.elementSize) + MetaSize;
	}
	return list;
}

list(void) ListPush(list(void) list, void * value) {
	return ListInsert(list, value, ListCount(list));
}

list(void) ListPop(list(void) list) {
	if (ListCount(list) == 0) {
		//printf("Trying to pop from an empty list.\n");
		exit(1);
	}
	return ListRemove(list, ListCount(list) - 1);
}

list(void) ListRemoveAll(list(void) list, void * value) {
	for (int i = 0; i < ListCount(list); i++) {
		if (memcmp((uint8_t *)list + i * ListElementSize(list), value, ListElementSize(list)) == 0) {
			list = ListRemove(list, i);
			i--;
		}
	}
	return list;
}

_Bool ListContains(list(void) list, void * value) {
	for (int i = 0; i < ListCount(list); i++) {
		if (memcmp((uint8_t *)list + i * ListElementSize(list), value, ListElementSize(list))) { return true; }
	}
	return false;
}

list(void) ListClear(list(void) list) {
	list(void) newList = ListCreate(ListElementSize(list));
	ListDestroy(list);
	return newList;
}

void ListDestroy(list(void) list) {
	free((uint8_t *)list - MetaSize);
}
