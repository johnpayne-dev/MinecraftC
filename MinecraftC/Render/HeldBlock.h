#pragma once
#include "../Level/Tile/Block.h"

typedef struct HeldBlock {
	struct Minecraft * minecraft;
	Block block;
	float position;
	float lastPosition;
	int offset;
	bool moving;
} HeldBlock;
