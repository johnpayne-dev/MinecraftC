#pragma once
#include "../Utilities/List.h"
#include "../GameSettings.h"
#include "Texture/AnimatedTexture.h"

typedef struct TextureManager {
	List(unsigned int) textures;
	List(char *) textureNames;
	uint8_t * textureBuffer;
	unsigned int idBuffer;
	List(AnimatedTexture *) animations;
	GameSettings * settings;
} TextureManager;

void TextureManagerCreate(TextureManager * manager, GameSettings * settings);
int TextureManagerLoad(TextureManager * manager, char * resource);
void TextureManagerReload(TextureManager * manager);
void TextureManagerRegisterAnimation(TextureManager * textures, AnimatedTexture * texture);
void TextureManagerDestroy(TextureManager * textures);
