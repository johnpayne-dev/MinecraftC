#include "PerlinNoise.h"
#include "../../../Utilities/Memory.h"

PerlinNoise PerlinNoiseCreate(RandomGenerator random) {
	Noise noise = NoiseCreate();
	noise->type = NoiseTypePerlin;
	noise->typeData = MemoryAllocate(sizeof(struct PerlinNoiseData));
	PerlinNoiseData this = noise->typeData;
	for (int i = 0; i < 256; i++) { this->hash[i] = i; }
	for (int i = 0; i < 256; i++) {
		int r1 = (int)RandomGeneratorIntegerRange(random, 0, 256 - i - 1) + i;
		int r2 = this->hash[i];
		this->hash[i] = this->hash[r1];
		this->hash[r1] = r2;
		this->hash[i + 256] = this->hash[i];
	}
	return noise;
}

static float3 F(float3 x) {
	return x * x * x * (x * (x * 6.0 - 15.0) + 10.0);
}

static float Lerp(float t, float a, float b) {
	return a + t * (b - a);
}

static float Grad(int i, float3 v) {
	i &= 15;
	float a = i < 8 ? v.x : v.y;
	float b = i < 4 ? v.y : (i != 12 && i != 14 ? v.z : v.x);
	return ((i & 1) == 0 ? a : -a) + ((i & 2) == 0 ? b : -b);
}

float PerlinNoiseCompute(PerlinNoise noise, float2 xy) {
	PerlinNoiseData this = noise->typeData;
	float3 v = { xy.x, xy.y, 0.0 };
	int3 vi = int3f(v) & 255;
	v -= (float3){ floorf(v.x), floorf(v.y), floorf(v.z) };
	float3 vd = F(v);
	int aaa, aba, aab, abb, baa, bba, bab, bbb;
	aaa = this->hash[this->hash[this->hash[vi.x] + vi.y] + vi.z];
	aba = this->hash[this->hash[this->hash[vi.x] + vi.y + 1] + vi.z];
	aab = this->hash[this->hash[this->hash[vi.x] + vi.y] + vi.z + 1];
	abb = this->hash[this->hash[this->hash[vi.x] + vi.y + 1] + vi.z + 1];
	baa = this->hash[this->hash[this->hash[vi.x + 1] + vi.y] + vi.z];
	bba = this->hash[this->hash[this->hash[vi.x + 1] + vi.y + 1] + vi.z];
	bab = this->hash[this->hash[this->hash[vi.x + 1] + vi.y] + vi.z + 1];
	bbb = this->hash[this->hash[this->hash[vi.x + 1] + vi.y + 1] + vi.z + 1];
	float l1 = Lerp(vd.x, Grad(aaa, v), Grad(baa, v - right3f));
	float l2 = Lerp(vd.x, Grad(aba, v - up3f), Grad(bba, v - right3f - up3f));
	float l3 = Lerp(vd.x, Grad(aab, v - forward3f), Grad(bab, v - right3f - forward3f));
	float l4 = Lerp(vd.x, Grad(abb, v - forward3f - up3f), Grad(bbb, v - one3f));
	float l = Lerp(vd.z, Lerp(vd.y, l1, l2), Lerp(vd.y, l3, l4));
	return l;
}

void PerlinNoiseDestroy(PerlinNoise noise) {
	PerlinNoiseData this = noise->typeData;
	MemoryFree(this);
}
