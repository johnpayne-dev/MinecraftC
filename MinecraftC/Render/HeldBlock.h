#pragma once
#include "../Level/Tile/Block.h"

typedef struct HeldBlock
{
	struct Minecraft * Minecraft;
	Block Block;
	float Position;
	float LastPosition;
	int Offset;
	bool Moving;
} HeldBlock;
