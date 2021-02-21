#pragma once
#include "../Utilities/LinearMath.h"

typedef struct NextTickListEntry
{
	int3 Position;
	BlockType Tile;
	int Ticks;
} NextTickListEntry;
