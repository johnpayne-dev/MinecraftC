#include "PerlinNoise.h"
#include <math.h>
#include <stdlib.h>

void PerlinNoiseCreate(PerlinNoise * noise, RandomGenerator * random) {
	NoiseCreate(noise);
	noise->type = NoiseTypePerlin;
	for (int i = 0; i < 256; i++) { noise->perlin.hash[i] = i; }
	for (int i = 0; i < 256; i++) {
		int r1 = (int)RandomGeneratorIntegerRange(random, 0, 256 - i - 1) + i;
		int r2 = noise->perlin.hash[i];
		noise->perlin.hash[i] = noise->perlin.hash[r1];
		noise->perlin.hash[r1] = r2;
		noise->perlin.hash[i + 256] = noise->perlin.hash[i];
	}
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

float PerlinNoiseCompute(PerlinNoise * noise, float x, float y) {
	float vx = x, vy = y, vz = 0.0;
	int ix = ((int)vx) & 255, iy = ((int)vy) & 255, iz = ((int)vz) & 255;
	vx -= floor(vx);
	vy -= floor(vy);
	vz -= floor(vz);
	float xd = F(vx), yd = F(vy), zd = F(vz);
	int aaa, aba, aab, abb, baa, bba, bab, bbb;
	aaa = noise->perlin.hash[noise->perlin.hash[noise->perlin.hash[ix] + iy] + iz];
	aba = noise->perlin.hash[noise->perlin.hash[noise->perlin.hash[ix] + iy + 1] + iz];
	aab = noise->perlin.hash[noise->perlin.hash[noise->perlin.hash[ix] + iy] + iz + 1];
	abb = noise->perlin.hash[noise->perlin.hash[noise->perlin.hash[ix] + iy + 1] + iz + 1];
	baa = noise->perlin.hash[noise->perlin.hash[noise->perlin.hash[ix + 1] + iy] + iz];
	bba = noise->perlin.hash[noise->perlin.hash[noise->perlin.hash[ix + 1] + iy + 1] + iz];
	bab = noise->perlin.hash[noise->perlin.hash[noise->perlin.hash[ix + 1] + iy] + iz + 1];
	bbb = noise->perlin.hash[noise->perlin.hash[noise->perlin.hash[ix + 1] + iy + 1] + iz + 1];
	float l1 = Lerp(xd, Grad(aaa, vx, vy, vz), Grad(baa, vx - 1.0, vy, vz));
	float l2 = Lerp(xd, Grad(aba, vx, vy - 1.0, vz), Grad(bba, vx - 1.0, vy - 1.0, vz));
	float l3 = Lerp(xd, Grad(aab, vx, vy, vz - 1.0), Grad(bab, vx - 1.0, vy, vz - 1.0));
	float l4 = Lerp(xd, Grad(abb, vx, vy - 1.0, vz - 1.0), Grad(bbb, vx - 1.0, vy - 1.0, vz - 1.0));
	float l = Lerp(zd, Lerp(yd, l1, l2), Lerp(yd, l3, l4));
	return l;
}
