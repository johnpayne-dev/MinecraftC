#pragma once
#include <stdlib.h>

void * MemoryAllocate(uint64_t size);

void MemoryFree(void * pointer);
