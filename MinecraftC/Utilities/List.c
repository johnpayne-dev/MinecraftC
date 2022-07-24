#include <string.h>
#include <stdlib.h>
#include "List.h"

struct ListInfo {
	uint32_t elementSize;
	uint32_t capacity;
	uint32_t length;
};

List(void) ListCreate(uint32_t elementSize) {
	struct ListInfo * list = malloc(sizeof(struct ListInfo) + elementSize);
	list->elementSize = elementSize;
	list->capacity = 1;
	list->length = 0;
	return list + 1;
}

uint32_t ListLength(List(void) list) {
	return ((struct ListInfo *)list - 1)->length;
}

uint32_t ListElementSize(List(void) list) {
	return ((struct ListInfo *)list - 1)->elementSize;
}

uint32_t ListCapacity(List(void) list) {
	return ((struct ListInfo *)list - 1)->capacity;
}

List(void) ListInsert(List(void) list, void * element, int32_t index) {
	struct ListInfo * info = (struct ListInfo *)list - 1;
	info->length++;
	if (info->length == info->capacity) {
		info->capacity *= 2;
		info = realloc(info, sizeof(struct ListInfo) + info->capacity * info->elementSize);
		list = info + 1;
	}
	memmove((uint8_t *)list + (index + 1) * info->elementSize, (uint8_t *)list + index * info->elementSize, (info->length - 1 - index) * info->elementSize);
	memcpy((uint8_t *)list + index * info->elementSize, element, info->elementSize);
	return list;
}

List(void) ListRemove(List(void) list, int32_t index) {
	struct ListInfo * info = (struct ListInfo *)list - 1;
	for (uint32_t j = (index + 1) * info->elementSize; j < info->length * info->elementSize; j++) {
		((uint8_t *)list)[j - info->elementSize] = ((uint8_t *)list)[j];
	}
	info->length--;
	if (info->length == info->capacity / 2 - 1) {
		info->capacity /= 2;
		info = realloc(info, sizeof(struct ListInfo) + info->capacity * info->elementSize);
		list = info + 1;
	}
	return list;
}

List(void) ListPush(List(void) list, void * value) {
	return ListInsert(list, value, ListLength(list));
}

List(void) ListPop(List(void) list) {
	return ListRemove(list, ListLength(list) - 1);
}

List(void) ListRemoveAll(List(void) list, void * value) {
	for (uint32_t i = 0; i < ListLength(list); i++) {
		if (memcmp((uint8_t *)list + i * ListElementSize(list), value, ListElementSize(list)) == 0) {
			list = ListRemove(list, i);
			i--;
		}
	}
	return list;
}

void ListSet(List(void) list, int32_t index, void * element) {
	struct ListInfo * info = (struct ListInfo *)list - 1;
	memcpy((uint8_t *)list + index * info->elementSize, element, info->elementSize);
}

void * ListGet(List(void) list, int32_t index) {
	struct ListInfo * info = (struct ListInfo *)list - 1;
	return (uint8_t *)list + index * info->elementSize;
}

int32_t ListIndexOf(List(void) list, void * value) {
	for (uint32_t i = 0; i < ListLength(list); i++) {
		if (memcmp((uint8_t *)list + i * ListElementSize(list), value, ListElementSize(list)) == 0) { return i; }
	}
	return -1;
}

bool ListContains(List(void) list, void * value) {
	return ListIndexOf(list, value) >= 0;
}

List(void) ListClear(List(void) list) {
	List(void) newList = ListCreate(ListElementSize(list));
	ListFree(list);
	return newList;
}

List(void) ListClone(List(void) list) {
	uint64_t size = sizeof(struct ListInfo) + ListElementSize(list) * ListCapacity(list);
	struct ListInfo * clone = malloc(size);
	memcpy(clone, (struct ListInfo *)list - 1, size);
	return clone + 1;
}

void ListFree(List(void) list) {
	free((struct ListInfo *)list - 1);
}
