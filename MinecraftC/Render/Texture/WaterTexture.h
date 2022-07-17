#pragma once
#include "AnimatedTexture.h"

typedef AnimatedTexture WaterTexture;

typedef struct WaterTextureData {
	float red[256];
	float green[256];
	float blue[256];
	float alpha[256];
	int updates;
} * WaterTextureData;

WaterTexture WaterTextureCreate(void);
void WaterTextureAnimate(WaterTexture texture);
void WaterTextureDestroy(WaterTexture texture);
