#include <string.h>
#include "TextureManager.h"
#include "../Utilities/Log.h"
#include "../Utilities/OpenGL.h"
#include "../../Resources/GUI/GUI.h"
#include "../../Resources/GUI/Icons.h"
#include "../../Resources/Clouds.h"
#include "../../Resources/Default.h"
#include "../../Resources/Dirt.h"
#include "../../Resources/Particles.h"
#include "../../Resources/Rain.h"
#include "../../Resources/Rock.h"
#include "../../Resources/Terrain.h"
#include "../../Resources/Water.h"
#include <stdlib.h>

void TextureManagerCreate(TextureManager * manager, GameSettings * settings) {
	*manager = (TextureManager) {
		.settings = settings,
		.animations = ListCreate(sizeof(AnimatedTexture *)),
		.textureBuffer = malloc(512 * 512),
		.textures = ListCreate(sizeof(unsigned int)),
		.textureNames = ListCreate(sizeof(char *)),
	};
}

int TextureManagerLoad(TextureManager * manager, char * resource) {
	bool loaded = false;
	int index = -1;
	for (int i = 0; i < ListLength(manager->textureNames); i++) {
		if (strcmp(manager->textureNames[i], resource) == 0) { loaded = true; index = i; }
	}
	if (loaded) { return manager->textures[index]; }
	
	int width = 0, height = 0;
	uint8_t * p = NULL;
	if (strcmp(resource, "Clouds.png") == 0) { p = Resource_Clouds_RGBA; width = Resource_Clouds_Width; height = Resource_Clouds_Height; }
	if (strcmp(resource, "Default.png") == 0) { p = Resource_Default_RGBA; width = Resource_Default_Width; height = Resource_Default_Height; }
	if (strcmp(resource, "Dirt.png") == 0) { p = Resource_Dirt_RGBA; width = Resource_Dirt_Width; height = Resource_Dirt_Height; }
	if (strcmp(resource, "Particles.png") == 0) { p = Resource_Particles_RGBA; width = Resource_Particles_Width; height = Resource_Particles_Height; }
	if (strcmp(resource, "Rain.png") == 0) { p = Resource_Rain_RGBA; width = Resource_Rain_Width; height = Resource_Rain_Height; }
	if (strcmp(resource, "Rock.png") == 0) { p = Resource_Rock_RGBA; width = Resource_Rock_Width; height = Resource_Rock_Height; }
	if (strcmp(resource, "Terrain.png") == 0) { p = Resource_Terrain_RGBA; width = Resource_Terrain_Width; height = Resource_Terrain_Height; }
	if (strcmp(resource, "Water.png") == 0) { p = Resource_Water_RGBA; width = Resource_Water_Width; height = Resource_Water_Height; }
	if (strcmp(resource, "GUI/GUI.png") == 0) { p = Resource_GUI_GUI_RGBA; width = Resource_GUI_GUI_Width; height = Resource_GUI_GUI_Height; }
	if (strcmp(resource, "GUI/Icons.png") == 0) { p = Resource_GUI_Icons_RGBA; width = Resource_GUI_Icons_Width; height = Resource_GUI_Icons_Height; }
	if (p == NULL) { LogFatal("Failed to load image %s.\n", resource); }
	
	uint8_t * pixels = malloc(4 * width * height);
	if (manager->settings->anaglyph) {
		for (int i = 0; i < 4 * width * height; i += 4) {
			uint8_t color[] = { (p[i + 0] * 30 + p[i + 1] * 59 + p[i + 2] * 11) / 100, (p[i + 0] * 30 + p[i + 1] * 70) / 100, (p[i + 0] * 30 + p[i + 2] * 70) / 100, p[i + 3] };
			memcpy(pixels + i, color, sizeof(color));
		}
	} else { memcpy(pixels, p, 4 * width * height); }
	
	glGenTextures(1, &manager->idBuffer);
	glBindTexture(GL_TEXTURE_2D, manager->idBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	free(pixels);
	
	manager->textureNames = ListPush(manager->textureNames, &resource);
	manager->textures = ListPush(manager->textures, &manager->idBuffer);
	return manager->idBuffer;
}

void TextureManagerReload(TextureManager * manager) {
	for (int i = 0; i < ListLength(manager->textures); i++) { glDeleteTextures(1, &manager->textures[i]); }
	manager->textures = ListClear(manager->textures);
	manager->textureNames = ListClear(manager->textureNames);
}

void TextureManagerRegisterAnimation(TextureManager * manager, AnimatedTexture * texture) {
	manager->animations = ListPush(manager->animations, &texture);
	AnimatedTextureAnimate(texture);
}

void TextureManagerDestroy(TextureManager * manager) {
	ListFree(manager->textures);
	ListFree(manager->textureNames);
	ListFree(manager->animations);
	free(manager->textureBuffer);
}

