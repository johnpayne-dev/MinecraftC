#pragma once
#include "../Utilities/List.h"
#include "../GameSettings.h"
#include "Texture/AnimatedTexture.h"

typedef struct TextureManager
{
	list(unsigned int) Textures;
	list(char *) TextureNames;
	uint8_t * TextureBuffer;
	unsigned int IDBuffer;
	list(AnimatedTexture) Animations;
	GameSettings Settings;
} * TextureManager;

TextureManager TextureManagerCreate(GameSettings settings);
int TextureManagerLoad(TextureManager textures, char * resource);
void TextureManagerReload(TextureManager manager);
void TextureManagerRegisterAnimation(TextureManager textures, AnimatedTexture texture);
void TextureManagerDestroy(TextureManager textures);
