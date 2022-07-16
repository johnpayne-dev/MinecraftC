#pragma once
#include "AnimatedTexture.h"

typedef AnimatedTexture WaterTexture;

typedef struct WaterTextureData {
	float4 colors[256];
	int updates;
} * WaterTextureData;

WaterTexture WaterTextureCreate(void);
void WaterTextureAnimate(WaterTexture texture);
void WaterTextureDestroy(WaterTexture texture);
