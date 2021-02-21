#include "LavaTexture.h"
#include "../../Level/Tile/Block.h"

LavaTexture LavaTextureCreate()
{
	LavaTexture texture = AnimatedTextureCreate(Blocks.Table[BlockTypeLava]->TextureID);
	texture->Type = AnimatedTextureTypeLava;
	texture->TypeData = MemoryAllocate(sizeof(struct LavaTextureData));
	*(LavaTextureData)texture->TypeData = (struct LavaTextureData){ 0 };
	return texture;
}

void LavaTextureAnimate(LavaTexture texture)
{
	LavaTextureData this = texture->TypeData;
	for (int x = 0; x < 16; x++)
	{
		for (int y = 0; y < 16; y++)
		{
			int sy = sin(y * pi / 8.0) * 1.2;
			int sx = sin(x * pi / 8.0) * 1.2;
			float v = 0.0;
			for (int i = x - 1; i <= x + 1; i++)
			{
				for (int j = y - 1; j <= y + 1; j++)
				{
					int a = i + sy & 15;
					int b = j + sx & 15;
					v += this->Colors[a + (b << 4)].r;
				}
			}
			this->Colors[x + (y << 4)].g = v / 10.0 + (this->Colors[(x & 15) + ((y & 15) << 4)].b + this->Colors[(x + 1 & 15) + ((y & 15) << 4)].b + this->Colors[(x + 1 & 15) + ((y + 1 & 15) << 4)].b + this->Colors[(x & 15) + ((y + 1 & 15) << 4)].b) / 4.0 * 0.8;
			this->Colors[x + (y << 4)].b += this->Colors[x + (y << 4)].a * 0.01;
			if (this->Colors[x + (y << 4)].b < 0.0) { this->Colors[x + (y << 4)].b = 0.0; }
			this->Colors[x + (y << 4)].a -= 0.06;
			if (RandomUniform() < 0.005) { this->Colors[x + (y << 4)].a = 1.5; }
		}
	}
	
	for (int i = 0; i < 256; i++)
	{
		this->Colors[i].rg = this->Colors[i].gr;
		float v = this->Colors[i].r * 2.0;
		if (v > 1.0) { v = 1.0; }
		if (v < 0.0) { v = 0.0; }
		int a = v * 100.0 + 155.0;
		int b = v * v * 255.0;
		int c = v * v * v * v * 128.0;
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
		texture->Data[(i << 2) + 3] = -1;
	}
}

void LavaTextureDestroy(LavaTexture texture)
{
	LavaTextureData this = texture->TypeData;
	MemoryFree(this);
}
