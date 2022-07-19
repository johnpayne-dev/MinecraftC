#include "WaterTexture.h"
#include "../../Level/Tile/Block.h"

void WaterTextureCreate(WaterTexture * texture) {
	AnimatedTextureCreate(texture, Blocks.table[BlockTypeWater]->textureID);
	texture->type = AnimatedTextureTypeWater;
	texture->water = (WaterTextureData){ 0 };
}

void WaterTextureAnimate(WaterTexture * texture) {
	WaterTextureData * this = &texture->water;
	this->updates++;
	
	for (int x = 0; x < 16; x++) {
		for (int y = 0; y < 16; y++) {
			float v = 0.0;
			for (int i = x - 1; i <= x + 1; i++) {
				int a = i & 15;
				int b = y & 15;
				v += this->red[a + (b << 4)];
			}
			this->blue[x + (y << 4)] = v / 3.3 + this->green[x + (y << 4)] * 0.8;
		}
	}
	for (int x = 0; x < 16; x++) {
		for (int y = 0; y < 16; y++) {
			this->green[x + (y << 4)] += this->alpha[x + (y << 4)] * 0.05;
			if (this->green[x + (y << 4)] < 0.0) { this->green[x + (y << 4)] = 0.0; }
			this->alpha[x + (y << 4)] -= 0.1;
			if (RandomUniform() < 0.05) { this->alpha[x + (y << 4)] = 0.5; }
		}
	}
	
	for (int i = 0; i < 256; i++) {
		float blue = this->blue[i];
		this->blue[i] = this->red[i];
		this->red[i] = blue;
		float v = this->red[i];
		if (v > 1.0) { v = 1.0; }
		if (v < 0.0) { v = 0.0; }
		int a = 32.0 + v * v * 32.0;
		int b = 50.0 + v * v * 64.0;
		int c = 255;
		int d = 146.0 + v * v * 50.0;
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
		texture->data[(i << 2) + 3] = d;
	}
}
