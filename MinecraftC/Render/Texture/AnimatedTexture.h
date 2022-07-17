#pragma once
#include <stdbool.h>
#include <stdint.h>

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
	void * typeData;
} * AnimatedTexture;

AnimatedTexture AnimatedTextureCreate(int textureID);
void AnimatedTextureAnimate(AnimatedTexture texture);
void AnimatedTextureDestroy(AnimatedTexture texture);
