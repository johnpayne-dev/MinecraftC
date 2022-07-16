#include "Memory.h"
#include "Log.h"

void * MemoryAllocate(uint64_t size) {
	void * ptr = malloc(size);
	if (ptr == NULL) { LogFatal("Failed to allocate memory size: %i\n", size); }
	return ptr;
}

void MemoryFree(void * pointer) {
	if (pointer != NULL) { free(pointer); }
	else { LogError("Tried to free NULL pointer.\n"); }
}
