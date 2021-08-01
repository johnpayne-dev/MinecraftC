#pragma once
#include <stdbool.h>
#include "../../Utilities/LinearMath.h"

typedef enum AnimatedTextureType
{
	AnimatedTextureTypeNone,
	AnimatedTextureTypeWater,
	AnimatedTextureTypeLava,
} AnimatedTextureType;

typedef struct AnimatedTexture
{
	uint8_t Data[1024];
	int TextureID;
	bool Anaglyph;
	AnimatedTextureType Type;
	void * TypeData;
} * AnimatedTexture;

AnimatedTexture AnimatedTextureCreate(int textureID);
void AnimatedTextureAnimate(AnimatedTexture texture);
void AnimatedTextureDestroy(AnimatedTexture texture);
