#include "PerlinNoise.h"
#include "../../../Utilities/Memory.h"

PerlinNoise PerlinNoiseCreate(RandomGenerator random)
{
	Noise noise = NoiseCreate();
	noise->Type = NoiseTypePerlin;
	noise->TypeData = MemoryAllocate(sizeof(struct PerlinNoiseData));
	PerlinNoiseData this = noise->TypeData;
	for (int i = 0; i < 256; i++) { this->Hash[i] = i; }
	for (int i = 0; i < 256; i++)
	{
		int r1 = (int)RandomGeneratorIntegerRange(random, 0, 256 - i - 1) + i;
		int r2 = this->Hash[i];
		this->Hash[i] = this->Hash[r1];
		this->Hash[r1] = r2;
		this->Hash[i + 256] = this->Hash[i];
	}
	return noise;
}

static float3 F(float3 x) { return x * x * x * (x * (x * 6.0 - 15.0) + 10.0); }
static float Lerp(float t, float a, float b) { return a + t * (b - a); }
static float Grad(int i, float3 v)
{
	i &= 15;
	float a = i < 8 ? v.x : v.y;
	float b = i < 4 ? v.y : (i != 12 && i != 14 ? v.z : v.x);
	return ((i & 1) == 0 ? a : -a) + ((i & 2) == 0 ? b : -b);
}

float PerlinNoiseCompute(PerlinNoise noise, float2 xy)
{
	PerlinNoiseData this = noise->TypeData;
	float3 v = { xy.x, xy.y, 0.0 };
	int3 vi = int3f(v) & 255;
	v -= (float3){ floorf(v.x), floorf(v.y), floorf(v.z) };
	float3 vd = F(v);
	int aaa, aba, aab, abb, baa, bba, bab, bbb;
	aaa = this->Hash[this->Hash[this->Hash[vi.x] + vi.y] + vi.z];
	aba = this->Hash[this->Hash[this->Hash[vi.x] + vi.y + 1] + vi.z];
	aab = this->Hash[this->Hash[this->Hash[vi.x] + vi.y] + vi.z + 1];
	abb = this->Hash[this->Hash[this->Hash[vi.x] + vi.y + 1] + vi.z + 1];
	baa = this->Hash[this->Hash[this->Hash[vi.x + 1] + vi.y] + vi.z];
	bba = this->Hash[this->Hash[this->Hash[vi.x + 1] + vi.y + 1] + vi.z];
	bab = this->Hash[this->Hash[this->Hash[vi.x + 1] + vi.y] + vi.z + 1];
	bbb = this->Hash[this->Hash[this->Hash[vi.x + 1] + vi.y + 1] + vi.z + 1];
	float l1 = Lerp(vd.x, Grad(aaa, v), Grad(baa, v - right3f));
	float l2 = Lerp(vd.x, Grad(aba, v - up3f), Grad(bba, v - right3f - up3f));
	float l3 = Lerp(vd.x, Grad(aab, v - forward3f), Grad(bab, v - right3f - forward3f));
	float l4 = Lerp(vd.x, Grad(abb, v - forward3f - up3f), Grad(bbb, v - one3f));
	float l = Lerp(vd.z, Lerp(vd.y, l1, l2), Lerp(vd.y, l3, l4));
	if (l > 1.0 || l < -1.0)
	{
		
	}
	return l;
}

void PerlinNoiseDestroy(PerlinNoise noise)
{
	PerlinNoiseData this = noise->TypeData;
	MemoryFree(this);
}
