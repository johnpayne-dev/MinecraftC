#pragma once
#include <stdlib.h>
#include <stdint.h>

void * MemoryAllocate(uint64_t size);

void MemoryFree(void * pointer);
