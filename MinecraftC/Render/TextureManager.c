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
		.Textures = ListCreate(sizeof(int)),
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
	unsigned char * pixels = stbi_load_from_memory(fileData, fileSize, &width, &height, &channels, 4);
	if (pixels == NULL) { LogFatal("Failed to open file %s: %s\n", resource, stbi_failure_reason()); }
	MemoryFree(fileData);
	
	glGenTextures(1, &manager->IDBuffer);
	glBindTexture(GL_TEXTURE_2D, manager->IDBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	stbi_image_free(pixels);
	/*
	 GL11.glBindTexture(3553, var2);
	       GL11.glTexParameteri(3553, 10241, 9728);
	       GL11.glTexParameteri(3553, 10240, 9728);
	       var2 = var1.getWidth();
	       int var3 = var1.getHeight();
	       int[] var4 = new int[var2 * var3];
	       byte[] var5 = new byte[var2 * var3 << 2];
	       var1.getRGB(0, 0, var2, var3, var4, 0, var2);

	       for(int var11 = 0; var11 < var4.length; ++var11) {
		  int var6 = var4[var11] >>> 24;
		  int var7 = var4[var11] >> 16 & 255;
		  int var8 = var4[var11] >> 8 & 255;
		  int var9 = var4[var11] & 255;
		  if(this.settings.anaglyph) {
		     int var10 = (var7 * 30 + var8 * 59 + var9 * 11) / 100;
		     var8 = (var7 * 30 + var8 * 70) / 100;
		     var9 = (var7 * 30 + var9 * 70) / 100;
		     var7 = var10;
		     var8 = var8;
		     var9 = var9;
		  }

		  var5[var11 << 2] = (byte)var7;
		  var5[(var11 << 2) + 1] = (byte)var8;
		  var5[(var11 << 2) + 2] = (byte)var9;
		  var5[(var11 << 2) + 3] = (byte)var6;
	       }

	       this.textureBuffer.clear();
	       this.textureBuffer.put(var5);
	       this.textureBuffer.position(0).limit(var5.length);
	       GL11.glTexImage2D(3553, 0, 6408, var2, var3, 0, 6408, 5121, this.textureBuffer);
	 */
	
	manager->TextureNames = ListPush(manager->TextureNames, &resource);
	manager->Textures = ListPush(manager->Textures, &manager->IDBuffer);
	return manager->IDBuffer;
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

