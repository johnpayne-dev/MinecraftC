#include "LavaTexture.h"
#include "../../Level/Tile/Block.h"
#include "../../Utilities/SinTable.h"

LavaTexture LavaTextureCreate() {
	LavaTexture texture = AnimatedTextureCreate(Blocks.table[BlockTypeLava]->textureID);
	texture->type = AnimatedTextureTypeLava;
	texture->typeData = MemoryAllocate(sizeof(struct LavaTextureData));
	*(LavaTextureData)texture->typeData = (struct LavaTextureData){ 0 };
	return texture;
}

void LavaTextureAnimate(LavaTexture texture) {
	LavaTextureData this = texture->typeData;
	for (int x = 0; x < 16; x++) {
		for (int y = 0; y < 16; y++) {
			int sy = tsin(y * M_PI / 8.0) * 1.2;
			int sx = tsin(x * M_PI / 8.0) * 1.2;
			float v = 0.0;
			for (int i = x - 1; i <= x + 1; i++) {
				for (int j = y - 1; j <= y + 1; j++) {
					int a = i + sy & 15;
					int b = j + sx & 15;
					v += this->red[a + (b << 4)];
				}
			}
			this->green[x + (y << 4)] = v / 10.0 + (this->blue[(x & 15) + ((y & 15) << 4)] + this->blue[(x + 1 & 15) + ((y & 15) << 4)] + this->blue[(x + 1 & 15) + ((y + 1 & 15) << 4)] + this->blue[(x & 15) + ((y + 1 & 15) << 4)]) / 4.0 * 0.8;
			this->blue[x + (y << 4)] += this->alpha[x + (y << 4)] * 0.01;
			if (this->blue[x + (y << 4)] < 0.0) { this->blue[x + (y << 4)] = 0.0; }
			this->alpha[x + (y << 4)] -= 0.06;
			if (RandomUniform() < 0.005) { this->alpha[x + (y << 4)] = 1.5; }
		}
	}
	
	for (int i = 0; i < 256; i++) {
		float red = this->red[i];
		this->red[i] = this->green[i];
		this->green[i] = red;
		float v = this->red[i] * 2.0;
		if (v > 1.0) { v = 1.0; }
		if (v < 0.0) { v = 0.0; }
		int a = v * 100.0 + 155.0;
		int b = v * v * 255.0;
		int c = v * v * v * v * 128.0;
		if (texture->anaglyph) {
			int aa = (a * 30 + b * 59 + c * 11) / 100;
			int bb = (a * 30 + b * 70) / 100;
			int cc = (a * 30 + c * 70) / 100;
			a = aa;
			b = bb;
			c = cc;
		}
		texture->data[i << 2] = a;
		texture->data[(i << 2) + 1] = b;
		texture->data[(i << 2) + 2] = c;
		texture->data[(i << 2) + 3] = -1;
	}
}

void LavaTextureDestroy(LavaTexture texture) {
	LavaTextureData this = texture->typeData;
	MemoryFree(this);
}
