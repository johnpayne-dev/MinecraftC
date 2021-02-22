#include <string.h>
#include "LinearMath.h"
#include "List.h"

static const unsigned int MetaSize = 2 * sizeof(unsigned int);

list(void) ListCreate(unsigned int elementSize)
{
	unsigned int * list = malloc(MetaSize + 2 * elementSize);
	list[0] = elementSize;
	list[1] = 0;
	return ((void *)list) + MetaSize;
}

struct ListData
{
	unsigned int ElementSize;
	unsigned int * Count;
	unsigned int Capacity;
};

static struct ListData ListData(list(void) list)
{
	struct ListData data = { 0 };
	data.ElementSize = *(unsigned int *)(list - MetaSize);
	data.Count = list - MetaSize / 2;
	data.Capacity = pow(2.0, floor(log2(*data.Count)) + 1.0);
	return data;
}

unsigned int ListCount(list(void) list)
{
	return *(unsigned int *)(list - MetaSize / 2);
}

unsigned int ListElementSize(list(void) list)
{
	return *(unsigned int *)(list - MetaSize);
}

unsigned int ListCapacity(list(void) list)
{
	return ListData(list).Capacity;
}

list(void) ListInsert(list(void) list, void * element, int index)
{
	struct ListData data = ListData(list);
	if (index < 0 || index > *data.Count)
	{
		//printf("Trying to add a value to a list, but the index is out of bounds.\n");
		exit(1);
	}
	
	(*data.Count)++;
	if (*data.Count == data.Capacity)
	{
		list = realloc(list - MetaSize, MetaSize + 2 * data.Capacity * data.ElementSize) + MetaSize;
	}
	data = ListData(list);
	memmove(list + (index + 1) * data.ElementSize, list + index * data.ElementSize, (*data.Count - 1 - index) * data.ElementSize);
	memcpy(list + index * data.ElementSize, element, data.ElementSize);
	return list;
}

list(void) ListRemove(list(void) list, int index)
{
	struct ListData data = ListData(list);
	if (index < 0 || index > *data.Count)
	{
		//printf("Trying to remove a value from a list, but the index is out of bounds.\n");
		exit(1);
	}

	for (int j = (index + 1) * (int)data.ElementSize; j < (*data.Count) * (int)data.ElementSize; j++)
	{
		((unsigned char *)list)[j - data.ElementSize] = ((unsigned char *)list)[j];
	}
	(*data.Count)--;
	if (*data.Count == data.Capacity / 2 - 1)
	{
		list = realloc(list - MetaSize, MetaSize + (data.Capacity / 2) * data.ElementSize) + MetaSize;
	}
	return list;
}

list(void) ListPush(list(void) list, void * value)
{
	return ListInsert(list, value, ListCount(list));
}

list(void) ListPop(list(void) list)
{
	if (ListCount(list) == 0)
	{
		//printf("Trying to pop from an empty list.\n");
		exit(1);
	}
	return ListRemove(list, ListCount(list) - 1);
}

list(void) ListRemoveAll(list(void) list, void * value)
{
	for (int i = 0; i < ListCount(list); i++)
	{
		if (memcmp(list + i * ListElementSize(list), value, ListElementSize(list)) == 0)
		{
			list = ListRemove(list, i);
			i--;
		}
	}
	return list;
}

_Bool ListContains(list(void) list, void * value)
{
	for (int i = 0; i < ListCount(list); i++)
	{
		if (memcmp(list + i * ListElementSize(list), value, ListElementSize(list))) { return true; }
	}
	return false;
}

list(void) ListClear(list(void) list)
{
	list(void) newList = ListCreate(ListElementSize(list));
	ListDestroy(list);
	return newList;
}

void ListDestroy(list(void) list)
{
	free(list - MetaSize);
}
