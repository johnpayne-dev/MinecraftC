#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "WaterTexture.h"
#include "LavaTexture.h"

typedef enum AnimatedTextureType {
	AnimatedTextureTypeNone,
	AnimatedTextureTypeWater,
	AnimatedTextureTypeLava,
} AnimatedTextureType;

typedef struct AnimatedTexture {
	uint8_t data[1024];
	int textureID;
	bool anaglyph;
	AnimatedTextureType type;
	union {
		WaterTextureData water;
		LavaTextureData lava;
	};
} AnimatedTexture;

void AnimatedTextureCreate(AnimatedTexture * texture, int textureID);
void AnimatedTextureAnimate(AnimatedTexture * texture);
