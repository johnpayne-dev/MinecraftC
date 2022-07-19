#pragma once

typedef struct AnimatedTexture LavaTexture;

typedef struct LavaTextureData {
	float red[256];
	float green[256];
	float blue[256];
	float alpha[256];
} LavaTextureData;

void LavaTextureCreate(LavaTexture * texture);
void LavaTextureAnimate(LavaTexture * texture);
