#include "PerlinNoise.h"
#include <math.h>
#include <stdlib.h>

PerlinNoise PerlinNoiseCreate(RandomGenerator random) {
	Noise noise = NoiseCreate();
	noise->type = NoiseTypePerlin;
	noise->typeData = malloc(sizeof(struct PerlinNoiseData));
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

static float F(float x) {
	return x * x * x * (x * (x * 6.0 - 15.0) + 10.0);
}

static float Lerp(float t, float a, float b) {
	return a + t * (b - a);
}

static float Grad(int i, float x, float y, float z) {
	i &= 15;
	float a = i < 8 ? x : y;
	float b = i < 4 ? y : (i != 12 && i != 14 ? z : x);
	return ((i & 1) == 0 ? a : -a) + ((i & 2) == 0 ? b : -b);
}

float PerlinNoiseCompute(PerlinNoise noise, float x, float y) {
	PerlinNoiseData this = noise->typeData;
	float vx = x, vy = y, vz = 0.0;
	int ix = ((int)vx) & 255, iy = ((int)vy) & 255, iz = ((int)vz) & 255;
	vx -= floor(vx);
	vy -= floor(vy);
	vz -= floor(vz);
	float xd = F(vx), yd = F(vy), zd = F(vz);
	int aaa, aba, aab, abb, baa, bba, bab, bbb;
	aaa = this->hash[this->hash[this->hash[ix] + iy] + iz];
	aba = this->hash[this->hash[this->hash[ix] + iy + 1] + iz];
	aab = this->hash[this->hash[this->hash[ix] + iy] + iz + 1];
	abb = this->hash[this->hash[this->hash[ix] + iy + 1] + iz + 1];
	baa = this->hash[this->hash[this->hash[ix + 1] + iy] + iz];
	bba = this->hash[this->hash[this->hash[ix + 1] + iy + 1] + iz];
	bab = this->hash[this->hash[this->hash[ix + 1] + iy] + iz + 1];
	bbb = this->hash[this->hash[this->hash[ix + 1] + iy + 1] + iz + 1];
	float l1 = Lerp(xd, Grad(aaa, vx, vy, vz), Grad(baa, vx - 1.0, vy, vz));
	float l2 = Lerp(xd, Grad(aba, vx, vy - 1.0, vz), Grad(bba, vx - 1.0, vy - 1.0, vz));
	float l3 = Lerp(xd, Grad(aab, vx, vy, vz - 1.0), Grad(bab, vx - 1.0, vy, vz - 1.0));
	float l4 = Lerp(xd, Grad(abb, vx, vy - 1.0, vz - 1.0), Grad(bbb, vx - 1.0, vy - 1.0, vz - 1.0));
	float l = Lerp(zd, Lerp(yd, l1, l2), Lerp(yd, l3, l4));
	return l;
}

void PerlinNoiseDestroy(PerlinNoise noise) {
	PerlinNoiseData this = noise->typeData;
	free(this);
}
