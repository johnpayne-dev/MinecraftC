#pragma once

typedef struct AnimatedTexture WaterTexture;

typedef struct WaterTextureData {
	float red[256];
	float green[256];
	float blue[256];
	float alpha[256];
	int updates;
} WaterTextureData;

void WaterTextureCreate(WaterTexture * texutre);
void WaterTextureAnimate(WaterTexture * texture);
