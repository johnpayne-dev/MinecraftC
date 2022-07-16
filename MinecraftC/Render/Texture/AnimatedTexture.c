#include "AnimatedTexture.h"
#include "LavaTexture.h"
#include "WaterTexture.h"
#include "../../Utilities/Memory.h"

AnimatedTexture AnimatedTextureCreate(int textureID) {
	AnimatedTexture texture = MemoryAllocate(sizeof(struct AnimatedTexture));
	*texture = (struct AnimatedTexture) {
		.textureID = textureID,
	};
	return texture;
}

void AnimatedTextureAnimate(AnimatedTexture texture) {
	if (texture->type == AnimatedTextureTypeLava) { LavaTextureAnimate(texture); return; }
	if (texture->type == AnimatedTextureTypeWater) { WaterTextureAnimate(texture); return; }
}

void AnimatedTextureDestroy(AnimatedTexture texture) {
	if (texture->type == AnimatedTextureTypeLava) { LavaTextureDestroy(texture); }
	if (texture->type == AnimatedTextureTypeWater) { WaterTextureDestroy(texture); }
	MemoryFree(texture);
}
