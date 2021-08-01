#include <string.h>
#include <OpenGL.h>
#include <stb_image.h>
#include "TextureManager.h"
#include "../Utilities/Log.h"

TextureManager TextureManagerCreate(GameSettings settings)
{
	TextureManager manager = MemoryAllocate(sizeof(struct TextureManager));
	*manager = (struct TextureManager)
	{
		.Settings = settings,
		.Animations = ListCreate(sizeof(AnimatedTexture)),
		.TextureBuffer = MemoryAllocate(512 * 512),
		.Textures = ListCreate(sizeof(unsigned int)),
		.TextureNames = ListCreate(sizeof(char *)),
	};
	return manager;
}

int TextureManagerLoad(TextureManager manager, char * resource)
{
	bool loaded = false;
	int index = -1;
	for (int i = 0; i < ListCount(manager->TextureNames); i++)
	{
		if (strcmp(manager->TextureNames[i], resource) == 0) { loaded = true; index = i; }
	}
	if (loaded) { return manager->Textures[index]; }
	
	SDL_RWops * file = SDL_RWFromFile(resource, "rb");
	if (file == NULL) { LogFatal("Fialed to open file %s: %s\n", resource, SDL_GetError()); }
	int fileSize = (int)SDL_RWseek(file, 0, RW_SEEK_END);
	SDL_RWseek(file, 0, RW_SEEK_SET);
	void * fileData = MemoryAllocate(fileSize);
	SDL_RWread(file, fileData, fileSize, 1);
	SDL_RWclose(file);
	
	int width, height, channels;
	uint8_t * p = stbi_load_from_memory(fileData, fileSize, &width, &height, &channels, 4);
	if (p == NULL) { LogFatal("Failed to open file %s: %s\n", resource, stbi_failure_reason()); }
	MemoryFree(fileData);
	
	if (manager->Settings->Anaglyph)
	{
		for (int i = 0; i < 4 * width * height; i += 4)
		{
			Color color = { (p[i + 0] * 30 + p[i + 1] * 59 + p[i + 2] * 11) / 100, (p[i + 0] * 30 + p[i + 1] * 70) / 100, (p[i + 0] * 30 + p[i + 2] * 70) / 100, p[i + 3] };
			memcpy(p + i, &color, sizeof(Color));
		}
	}
	
	glGenTextures(1, &manager->IDBuffer);
	glBindTexture(GL_TEXTURE_2D, manager->IDBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, p);
	stbi_image_free(p);
	
	manager->TextureNames = ListPush(manager->TextureNames, &resource);
	manager->Textures = ListPush(manager->Textures, &manager->IDBuffer);
	return manager->IDBuffer;
}

void TextureManagerReload(TextureManager manager)
{
	for (int i = 0; i < ListCount(manager->Textures); i++) { glDeleteTextures(1, &manager->Textures[i]); }
	manager->Textures = ListClear(manager->Textures);
	manager->TextureNames = ListClear(manager->TextureNames);
}

void TextureManagerRegisterAnimation(TextureManager manager, AnimatedTexture texture)
{
	manager->Animations = ListPush(manager->Animations, &texture);
	AnimatedTextureAnimate(texture);
}

void TextureManagerDestroy(TextureManager manager)
{
	ListDestroy(manager->Textures);
	ListDestroy(manager->TextureNames);
	ListDestroy(manager->Animations);
	MemoryFree(manager->TextureBuffer);
	MemoryFree(manager);
}

