#include "AnimatedTexture.h"
#include "LavaTexture.h"
#include "WaterTexture.h"
#include <stdlib.h>

void AnimatedTextureCreate(AnimatedTexture * texture, int textureID) {
	*texture = (AnimatedTexture) {
		.textureID = textureID,
	};
}

void AnimatedTextureAnimate(AnimatedTexture * texture) {
	if (texture->type == AnimatedTextureTypeLava) { LavaTextureAnimate(texture); return; }
	if (texture->type == AnimatedTextureTypeWater) { WaterTextureAnimate(texture); return; }
}
