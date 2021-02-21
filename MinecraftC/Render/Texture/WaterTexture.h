#pragma once
#include "AnimatedTexture.h"

typedef AnimatedTexture WaterTexture;

typedef struct WaterTextureData
{
	float4 Colors[256];
	int Updates;
} * WaterTextureData;

WaterTexture WaterTextureCreate(void);
void WaterTextureAnimate(WaterTexture texture);
void WaterTextureDestroy(WaterTexture texture);
