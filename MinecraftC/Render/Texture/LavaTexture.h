#pragma once
#include "AnimatedTexture.h"

typedef AnimatedTexture LavaTexture;

typedef struct LavaTextureData {
	float red[256];
	float green[256];
	float blue[256];
	float alpha[256];
} * LavaTextureData;

LavaTexture LavaTextureCreate(void);
void LavaTextureAnimate(LavaTexture texture);
void LavaTextureDestroy(LavaTexture texture);
