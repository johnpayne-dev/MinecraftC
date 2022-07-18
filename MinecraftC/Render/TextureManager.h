#pragma once
#include "../Utilities/List.h"
#include "../GameSettings.h"
#include "Texture/AnimatedTexture.h"

typedef struct TextureManager {
	List(unsigned int) textures;
	List(char *) textureNames;
	uint8_t * textureBuffer;
	unsigned int idBuffer;
	List(AnimatedTexture) animations;
	GameSettings settings;
} * TextureManager;

TextureManager TextureManagerCreate(GameSettings settings);
int TextureManagerLoad(TextureManager textures, char * resource);
void TextureManagerReload(TextureManager manager);
void TextureManagerRegisterAnimation(TextureManager textures, AnimatedTexture texture);
void TextureManagerDestroy(TextureManager textures);
