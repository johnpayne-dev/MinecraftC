#pragma once

#include <stdint.h>
#include <stdbool.h>

#define List(type) type*

List(void) ListCreate(uint32_t elementSize);

uint32_t ListLength(List(void) list);

uint32_t ListElementSize(List(void) list);

uint32_t ListCapacity(List(void) list);

List(void) ListInsert(List(void) list, void * value, int32_t index);

List(void) ListRemove(List(void) list, int32_t index);

List(void) ListPush(List(void) list, void * value);

List(void) ListPop(List(void) list);

List(void) ListRemoveAll(List(void) list, void * value);

List(void) ListRemoveFirst(List(void) list, void * value);

List(void) ListRemoveLast(List(void) list, void * value);

void ListSet(List(void) list, int32_t index, void * value);

void * ListGet(List(void) list, int32_t index);

int32_t ListIndexOf(List(void) list, void * value);

bool ListContains(List(void) list, void * value);

List(void) ListClear(List(void) list);

List(void) ListClone(List(void) list);

void ListFree(List(void) list);

