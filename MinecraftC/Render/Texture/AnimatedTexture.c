#include "AnimatedTexture.h"
#include "LavaTexture.h"
#include "WaterTexture.h"
#include "../../Utilities/Memory.h"

AnimatedTexture AnimatedTextureCreate(int textureID)
{
	AnimatedTexture texture = MemoryAllocate(sizeof(struct AnimatedTexture));
	*texture = (struct AnimatedTexture)
	{
		.TextureID = textureID,
	};
	return texture;
}

void AnimatedTextureAnimate(AnimatedTexture texture)
{
	if (texture->Type == AnimatedTextureTypeLava) { LavaTextureAnimate(texture); return; }
	if (texture->Type == AnimatedTextureTypeWater) { WaterTextureAnimate(texture); return; }
}

void AnimatedTextureDestroy(AnimatedTexture texture)
{
	if (texture->Type == AnimatedTextureTypeLava) { LavaTextureDestroy(texture); }
	if (texture->Type == AnimatedTextureTypeWater) { WaterTextureDestroy(texture); }
	MemoryFree(texture);
}
