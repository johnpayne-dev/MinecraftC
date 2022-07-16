#pragma once
#include "../Utilities/List.h"
#include "../GameSettings.h"
#include "Texture/AnimatedTexture.h"

typedef struct TextureManager {
	list(unsigned int) textures;
	list(char *) textureNames;
	uint8_t * textureBuffer;
	unsigned int idBuffer;
	list(AnimatedTexture) animations;
	GameSettings settings;
} * TextureManager;

TextureManager TextureManagerCreate(GameSettings settings);
int TextureManagerLoad(TextureManager textures, char * resource);
void TextureManagerReload(TextureManager manager);
void TextureManagerRegisterAnimation(TextureManager textures, AnimatedTexture texture);
void TextureManagerDestroy(TextureManager textures);
