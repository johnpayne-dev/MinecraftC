#include "WaterTexture.h"
#include "../../Level/Tile/Block.h"

WaterTexture WaterTextureCreate()
{
	WaterTexture texture = AnimatedTextureCreate(Blocks.Table[BlockTypeWater]->TextureID);
	texture->Type = AnimatedTextureTypeWater;
	texture->TypeData = MemoryAllocate(sizeof(struct WaterTextureData));
	*(WaterTextureData)texture->TypeData = (struct WaterTextureData){ 0 };
	return texture;
}

void WaterTextureAnimate(WaterTexture texture)
{
	WaterTextureData this = texture->TypeData;
	this->Updates++;
	
	for (int x = 0; x < 16; x++)
	{
		for (int y = 0; y < 16; y++)
		{
			float v = 0.0;
			for (int i = x - 1; i <= x + 1; i++)
			{
				int a = i & 15;
				int b = y & 15;
				v += this->Colors[a + (b << 4)].r;
			}
			this->Colors[x + (y << 4)].b = v / 3.3 + this->Colors[x + (y << 4)].g * 0.8;
		}
	}
	for (int x = 0; x < 16; x++)
	{
		for (int y = 0; y < 16; y++)
		{
			this->Colors[x + (y << 4)].g += this->Colors[x + (y << 4)].a * 0.05;
			if (this->Colors[x + (y << 4)].g < 0.0) { this->Colors[x + (y << 4)].g = 0.0; }
			this->Colors[x + (y << 4)].a -= 0.1;
			if (RandomUniform() < 0.05) { this->Colors[x + (y << 4)].a = 0.5; }
		}
	}
	
	for (int i = 0; i < 256; i++)
	{
		this->Colors[i].rb = this->Colors[i].br;
		float v = this->Colors[i].r;
		if (v > 1.0) { v = 1.0; }
		if (v < 0.0) { v = 0.0; }
		int a = 32.0 + v * v * 32.0;
		int b = 50.0 + v * v * 64.0;
		int c = 255;
		int d = 146.0 + v * v * 50.0;
		if (texture->Anaglyph)
		{
			int aa = (a * 30 + b * 59 + c * 11) / 100;
			int bb = (a * 30 + b * 70) / 100;
			int cc = (a * 30 + c * 70) / 100;
			a = aa;
			b = bb;
			c = cc;
		}
		texture->Data[i << 2] = a;
		texture->Data[(i << 2) + 1] = b;
		texture->Data[(i << 2) + 2] = c;
		texture->Data[(i << 2) + 3] = d;
	}
}

void WaterTextureDestroy(WaterTexture texture)
{
	WaterTextureData this = texture->TypeData;
	MemoryFree(this);
}
