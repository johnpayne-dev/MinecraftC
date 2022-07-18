#include "LevelGenerator.h"
#include "Noise/Noise.h"
#include "Noise/OctaveNoise.h"
#include "Noise/CombinedNoise.h"
#include "../../Utilities/Time.h"
#include "../../Utilities/Log.h"
#include "../../Utilities/SinTable.h"

LevelGenerator LevelGeneratorCreate(ProgressBarDisplay progressBar) {
	LevelGenerator generator = malloc(sizeof(struct LevelGenerator));
	*generator = (struct LevelGenerator) {
		.progressBar = progressBar,
		.random = RandomGeneratorCreate(time(NULL)),
		.floodData = malloc(1024 * 1024 * sizeof(int)),
	};
	return generator;
}

static void PopulateOre(LevelGenerator generator, BlockType ore, int a1, int a2, int a3) {
	int w = generator->width;
	int h = generator->height;
	int d = generator->depth;
	int ii = w * d * h / 256 / 64 * a1 / 100;
	for (int i = 0; i < ii; i++) {
		ProgressBarDisplaySetProgress(generator->progressBar, i * 100 / (ii - 1) / 4 + a2 * 100 / 4);
		float x1 = RandomGeneratorUniform(generator->random) * w;
		float y1 = RandomGeneratorUniform(generator->random) * h;
		float z1 = RandomGeneratorUniform(generator->random) * d;
		int jj = (RandomGeneratorUniform(generator->random) + RandomGeneratorUniform(generator->random)) * 75.0 * a1 / 100.0;
		float r1x = RandomGeneratorUniform(generator->random) * 2.0 * M_PI;
		float r1y = RandomGeneratorUniform(generator->random) * 2.0 * M_PI;
		float r2x = 0.0;
		float r2y = 0.0;
		for (int j = 0; j < jj; j++) {
			x1 += tsin(r1x) * tcos(r1y);
			y1 += tsin(r1y);
			z1 += tcos(r1x) * tcos(r1y);
			r1x += r2x * 0.2;
			r2x = r2x * 0.9 + RandomGeneratorUniform(generator->random) - RandomGeneratorUniform(generator->random);
			r1y = (r1y + r2y * 0.5) * 0.5;
			r2y = r2y * 0.9 + RandomGeneratorUniform(generator->random) - RandomGeneratorUniform(generator->random);
			float v2 = tsin(j * M_PI / jj) * a1 / 100.0 + 1.0;
			for (int x = x1 - v2; x <= (int)(x1 + v2); x++) {
				for (int y = y1 - v2; y <= (int)(y1 + v2); y++) {
					for (int z = z1 - v2; z <= (int)(z1 + v2); z++) {
						float x2 = x - x1;
						float y2 = y - y1;
						float z2 = z - z1;
						if (x2 * x2 + 2.0 * y2 * y2 + z2 * z2 < v2 * v2 && x >= 1 && y >= 1 && z >= 1 && x < w - 1 && y < h - 1 && z < d - 1) {
							int c = (y * d + z) * w + x;
							if (generator->blocks[c] == BlockTypeStone) { generator->blocks[c] = ore; }
						}
					}
				}
			}
		}
	}
}

static int64_t Flood(LevelGenerator generator, int x, int y, int z, int var, BlockType tile) {
	List(int *) floodStack = ListCreate(sizeof(int *));
	
	int xx = 1, zz = 1;
	for (xx = 1; 1 << zz < generator->width; zz++);
	while (1 << xx < generator->depth) { xx++; }
	int d = generator->depth - 1;
	int w = generator->width - 1;
	generator->floodData[0] = (((y << xx) + z) << zz) + x;
	int i = 1;
	int64_t j = 0;
	int ii = generator->width * generator->depth;
	while (i > 0) {
		i--;
		int f = generator->floodData[i];
		if (i == 0 && ListLength(floodStack) > 0) {
			free(generator->floodData);
			generator->floodData = floodStack[ListLength(floodStack) - 1];
			floodStack = ListRemove(floodStack, ListLength(floodStack) - 1);
		}
		int v1 = f >> zz & d;
		int v2 = f >> (zz + xx);
		int v3 = f & w, v4 = f & w;
		for (v3 = v3; v3 > 0 && generator->blocks[f - 1] == 0; f--) { v3--; }
		while (v4 < generator->width && generator->blocks[f + v4 - v3] == 0) { v4++; }
		int v5 = f >> zz & d;
		int v6 = f >> (zz + xx);
		if (v5 != v1 || v6 != v2) { LogDebug("Diagonal flood!?\n"); }
		
		bool b1 = false, b2 = false, b3 = false;
		j += v4 - v3;
		for (v3 = v3; v3 < v4; v3++) {
			generator->blocks[f] = tile;
			if (v1 > 0) {
				bool b4 = generator->blocks[f - generator->width] == 0;
				if (b4 && !b1) {
					if (i == 1024 * 1024) {
						floodStack = ListPush(floodStack, &generator->floodData);
						generator->floodData = malloc(1024 * 1024 * sizeof(int));
						i = 0;
					}
					generator->floodData[i++] = f - generator->width;
				}
				b1 = b4;
			}
			if (v1 < generator->depth - 1) {
				bool b4 = generator->blocks[f + generator->width] == 0;
				if (b4 && !b2) {
					if (i == 1024 * 1024) {
						floodStack = ListPush(floodStack, &generator->floodData);
						generator->floodData = malloc(1024 * 1024 * sizeof(int));
						i = 0;
					}
					generator->floodData[i++] = f + generator->width;
				}
				b2 = b4;
			}
			if (v2 > 0) {
				BlockType t = generator->blocks[f - ii];
				if ((tile == BlockTypeLava || tile == BlockTypeStillLava) && (t == BlockTypeWater || t == BlockTypeStillWater)) { generator->blocks[f - ii] = BlockTypeStone; }
				bool b4 = t == 0;
				if (b4 && !b3) {
					if (i == 1024 * 1024) {
						floodStack = ListPush(floodStack, &generator->floodData);
						generator->floodData = malloc(1024 * 1024 * sizeof(int));
						i = 0;
					}
					generator->floodData[i++] = f - ii;
				}
				b3 = b4;
			}
			f++;
		}
	}
	for (int i = 0; i < ListLength(floodStack); i++) { free(floodStack[i]); }
	ListFree(floodStack);
	return j;
}

Level LevelGeneratorGenerate(LevelGenerator generator, const char * userName, int width, int depth) {
	ProgressBarDisplaySetTitle(generator->progressBar, "Generating level");
	generator->width = width;
	generator->depth = depth;
	generator->height = 64;
	generator->waterLevel = 32;
	generator->blocks = malloc(width * depth << 6);
	int w = generator->width;
	int h = generator->height;
	int d = generator->depth;
	
	ProgressBarDisplaySetText(generator->progressBar, "Raising..");
	CombinedNoise n1 = CombinedNoiseCreate(OctaveNoiseCreate(generator->random, 8), OctaveNoiseCreate(generator->random, 8));
	CombinedNoise n2 = CombinedNoiseCreate(OctaveNoiseCreate(generator->random, 8), OctaveNoiseCreate(generator->random, 8));
	OctaveNoise n3 = OctaveNoiseCreate(generator->random, 6);
	int * heights = malloc(w * d * sizeof(int));
	for (int x = 0; x < w; x++) {
		ProgressBarDisplaySetProgress(generator->progressBar, x * 100 / (w - 1));
		for (int y = 0; y < d; y++) {
 			float v1 = NoiseCompute(n1, x * 1.3, y * 1.3) / 6.0 - 4.0;
			float v2 = NoiseCompute(n2, x * 1.3, y * 1.3) / 5.0 + 6.0;
			if (NoiseCompute(n3, x, y) / 8.0 > 0.0) { v2 = v1; }
			float m = fmaxf(v1, v2) / 2.0;
			if (m < 0.0) { m *= 0.8; }
			heights[x + y * w] = m;
		}
	}
	NoiseDestroy(((CombinedNoiseData)n1->typeData)->noise1);
	NoiseDestroy(((CombinedNoiseData)n1->typeData)->noise2);
	NoiseDestroy(n1);
	NoiseDestroy(((CombinedNoiseData)n2->typeData)->noise1);
	NoiseDestroy(((CombinedNoiseData)n2->typeData)->noise2);
	NoiseDestroy(n2);
	NoiseDestroy(n3);
	
	ProgressBarDisplaySetText(generator->progressBar, "Eroding..");
	n1 = CombinedNoiseCreate(OctaveNoiseCreate(generator->random, 8), OctaveNoiseCreate(generator->random, 8));
	n2 = CombinedNoiseCreate(OctaveNoiseCreate(generator->random, 8), OctaveNoiseCreate(generator->random, 8));
	for (int x = 0; x < w; x++) {
		ProgressBarDisplaySetProgress(generator->progressBar, x * 100 / (w - 1));
		for (int y = 0; y < d; y++)
		{
			float v1 = NoiseCompute(n1, x * 2.0, y * 2.0) / 8.0;
			int v2 = NoiseCompute(n2, x * 2.0, y * 2.0) > 0.0 ? 1 : 0;
			if (v1 > 2.0) { heights[x + y * w] = ((heights[x + y * w] - v2) / 2 << 1) + v2; }
		}
	}
	NoiseDestroy(((CombinedNoiseData)n1->typeData)->noise1);
	NoiseDestroy(((CombinedNoiseData)n1->typeData)->noise2);
	NoiseDestroy(n1);
	NoiseDestroy(((CombinedNoiseData)n2->typeData)->noise1);
	NoiseDestroy(((CombinedNoiseData)n2->typeData)->noise2);
	NoiseDestroy(n2);
	
	ProgressBarDisplaySetText(generator->progressBar, "Soiling..");
	OctaveNoise n = OctaveNoiseCreate(generator->random, 6);
	for (int x = 0; x < w; x++) {
		ProgressBarDisplaySetProgress(generator->progressBar, x * 100 / (w - 1));
		for (int y = 0; y < d; y++) {
			int v1 = (int)(NoiseCompute(n, x, y) / 24.0) - 4;
			int v2 = heights[x + y * w] + generator->waterLevel;
			int v3 = v2 + v1;
			heights[x + y * w] = v2 > v3 ? v2 : v3;
			if (heights[x + y * w] > h - 2) { heights[x + y * w] = h - 2; }
			if (heights[x + y * w] < 1) { heights[x + y * w] = 1; }
			for (int z = 0; z < h; z++) {
				int c = (z * d + y) * w + x;
				BlockType tile = BlockTypeNone;
				if (z <= v2) { tile = BlockTypeDirt; }
				if (z <= v3) { tile = BlockTypeStone; }
				if (z == 0) { tile = BlockTypeLava; }
				generator->blocks[c] = tile;
			}
		}
	}
	NoiseDestroy(n);
	
	ProgressBarDisplaySetText(generator->progressBar, "Carving..");
	int ii = w * d * h / 256 / 64 << 1;
	for (int i = 0; i < ii; i++) {
		ProgressBarDisplaySetProgress(generator->progressBar, i * 100 / (ii - 1));
		
		float x1 = RandomGeneratorUniform(generator->random) * w;
		float y1 = RandomGeneratorUniform(generator->random) * h;
		float z1 = RandomGeneratorUniform(generator->random) * d;
		int jj = (RandomGeneratorUniform(generator->random) + RandomGeneratorUniform(generator->random)) * 200.0;
		float r1x = RandomGeneratorUniform(generator->random) * 2.0 * M_PI;
		float r1y = RandomGeneratorUniform(generator->random) * 2.0 * M_PI;
		float r2x = 0.0;
		float r2y = 0.0;
		float v2 = RandomGeneratorUniform(generator->random) * RandomGeneratorUniform(generator->random);
		for (int j = 0; j < jj; j++) {
			x1 += tsin(r1x) * tcos(r1y);
			y1 += tsin(r1y);
			z1 += tcos(r1x) * tcos(r1y);
			r1x = (r1x + r1x * 0.2) * 0.9;
			r2x = r1x + RandomGeneratorUniform(generator->random) - RandomGeneratorUniform(generator->random);
			r1y = (r1y + r2y * 0.5) * 0.5;
			r2y = r2y * 0.75 + RandomGeneratorUniform(generator->random) - RandomGeneratorUniform(generator->random);
			if (RandomGeneratorUniform(generator->random) >= 0.25) {
				float x2 = x1 + (RandomGeneratorUniform(generator->random) * 4.0 - 2.0) * 0.2;
				float y2 = y1 + (RandomGeneratorUniform(generator->random) * 4.0 - 2.0) * 0.2;
				float z2 = z1 + (RandomGeneratorUniform(generator->random) * 4.0 - 2.0) * 0.2;
				float v4 = (h - y2) / h;
				v4 = 1.2 + (v4 * 3.5 + 1.0) * v2;
				v4 *= tsin(j * M_PI / jj);
				for (int x = x2 - v4; x <= (int)(x2 + v4); x++) {
					for (int y = y2 - v4; y <= (int)(y2 + v4); y++) {
						for (int z = z2 - v4; z <= (int)(z2 + v4); z++) {
							float x3 = x - x2;
							float y3 = y - y2;
							float z3 = z - z2;
							if (x3 * x3 + 2.0 * y3 * y3 + z3 * z3 < v4 * v4 && x >= 1 && y >= 1 && z >= 1 && x < w - 1 && y < h - 1 && z < d - 1) {
								int c = (y * d + z) * w + x;
								if (generator->blocks[c] == BlockTypeStone) { generator->blocks[c] = BlockTypeNone; }
							}
						}
					}
				}
			}
		}
	}
	PopulateOre(generator, BlockTypeCoalOre, 90, 1, 4);
	PopulateOre(generator, BlockTypeIronOre, 70, 2, 4);
	PopulateOre(generator, BlockTypeGoldOre, 50, 3, 4);
	
	ProgressBarDisplaySetText(generator->progressBar, "Watering..");
	ProgressBarDisplaySetProgress(generator->progressBar, 0);
	BlockType flood = BlockTypeStillWater;
	for (int x = 0; x < w; x++) {
		Flood(generator, x, h / 2 - 1, 0, 0, flood);
		Flood(generator, x, h / 2 - 1, d - 1, 0, flood);
	}
	for (int z = 0; z < d; z++) {
		Flood(generator, 0, h / 2 - 1, z, 0, flood);
		Flood(generator, w - 1, h / 2 - 1, z, 0, flood);
	}
	ii = w * d / 8000;
	for (int i = 0; i < ii; i++) {
		if (i % 100 == 0) { ProgressBarDisplaySetProgress(generator->progressBar, i * 100 / (ii - 1)); }
		int x = (int)RandomGeneratorIntegerRange(generator->random, 0, w - 1);
		int y = generator->waterLevel - 1 - (int)RandomGeneratorIntegerRange(generator->random, 0, 1);
		int z = (int)RandomGeneratorIntegerRange(generator->random, 0, d - 1);
		if (generator->blocks[(y * d + z) * w + x] == BlockTypeNone) { Flood(generator, x, y, z, 0, flood); }
	}
	ProgressBarDisplaySetProgress(generator->progressBar, 100);
	
	ProgressBarDisplaySetText(generator->progressBar, "Melting..");
	ii = w * d * h / 20000;
	for (int i = 0; i < ii; i++) {
		if (i % 100 == 0) { ProgressBarDisplaySetProgress(generator->progressBar, i * 100 / (ii - 1)); }
		int x = (int)RandomGeneratorIntegerRange(generator->random, 0, w - 1);
		int y = RandomGeneratorUniform(generator->random) * RandomGeneratorUniform(generator->random) * (generator->waterLevel - 3);
		int z = (int)RandomGeneratorIntegerRange(generator->random, 0, d - 1);
		if (generator->blocks[(y * d + z) * w + x] == BlockTypeNone) { Flood(generator, x, y, z, 0, BlockTypeStillLava); }
	}
	ProgressBarDisplaySetProgress(generator->progressBar, 100);
	
	ProgressBarDisplaySetText(generator->progressBar, "Growing..");
	n1 = OctaveNoiseCreate(generator->random, 8);
	n2 = OctaveNoiseCreate(generator->random, 8);
	for (int x = 0; x < w; x++) {
		ProgressBarDisplaySetProgress(generator->progressBar, x * 100 / (w - 1));
		for (int y = 0; y < d; y++) {
			bool v1 = NoiseCompute(n1, x, y) > 8.0;
			bool v2 = NoiseCompute(n2, x, y) > 12.0;
			int z = heights[x + y * w];
			int c = (z * d + y) * w + x;
			BlockType above = generator->blocks[((z + 1) * d + y) * w + x];
			if ((above == BlockTypeWater || above == BlockTypeStillWater) && z <= h / 2 - 1 && v2) { generator->blocks[c] = BlockTypeGravel; }
			if (above == BlockTypeNone) {
				BlockType tile = BlockTypeGrass;
				if (z <= h / 2 - 1 && v1) { tile = BlockTypeSand; }
				generator->blocks[c] = tile;
			}
		}
	}
	NoiseDestroy(n1);
	NoiseDestroy(n2);
	
	ProgressBarDisplaySetText(generator->progressBar, "Planting..");
	ii = w * d / 3000;
	for (int i = 0; i < ii; i++) {
		ProgressBarDisplaySetProgress(generator->progressBar, i * 50 / (ii - 1));
		int x = (int)RandomGeneratorIntegerRange(generator->random, 0, w - 1);
		int z = (int)RandomGeneratorIntegerRange(generator->random, 0, d - 1);
		int f = (int)RandomGeneratorIntegerRange(generator->random, 0, 1);
		for (int j = 0; j < 10; j++) {
			int xx = x;
			int zz = z;
			for (int k = 0; k < 5; k++) {
				xx += (int)RandomGeneratorIntegerRange(generator->random, 0, 5) - (int)RandomGeneratorIntegerRange(generator->random, 0, 5);
				zz += (int)RandomGeneratorIntegerRange(generator->random, 0, 5) - (int)RandomGeneratorIntegerRange(generator->random, 0, 5);
				if ((f < 2 || RandomGeneratorIntegerRange(generator->random, 0, 3) == 0) && xx >= 0 && zz >= 0 && xx < w && zz < d) {
					int y = heights[xx + zz * width] + 1;
					int c = (y * d + zz) * w + xx;
					if (generator->blocks[c] == BlockTypeNone) {
						if (generator->blocks[((y - 1) * d + zz) * w + xx] == BlockTypeGrass) {
							if (f == 0) { generator->blocks[c] = BlockTypeDandelion; }
							else if (f == 1) { generator->blocks[c] = BlockTypeRose; }
						}
					}
				}
			}
		}
	}
	ii = w * d * h / 2000;
	for (int i = 0; i < ii; i++) {
		ProgressBarDisplaySetProgress(generator->progressBar, i * 50 / (ii - 1) + 50);
		int m = (int)RandomGeneratorIntegerRange(generator->random, 0, 1);
		int x = (int)RandomGeneratorIntegerRange(generator->random, 0, w - 1);
		int y = (int)RandomGeneratorIntegerRange(generator->random, 0, h - 1);
		int z = (int)RandomGeneratorIntegerRange(generator->random, 0, d - 1);
		for (int j = 0; j < 20; j++) {
			int xx = x;
			int yy = y;
			int zz = z;
			for (int k = 0; k < 5; k++) {
				xx += (int)RandomGeneratorIntegerRange(generator->random, 0, 5) - (int)RandomGeneratorIntegerRange(generator->random, 0, 5);
				yy += (int)RandomGeneratorIntegerRange(generator->random, 0, 1) - (int)RandomGeneratorIntegerRange(generator->random, 0, 1);
				zz += (int)RandomGeneratorIntegerRange(generator->random, 0, 5) - (int)RandomGeneratorIntegerRange(generator->random, 0, 5);
				int c = (yy * d + zz) * w + xx;
				if ((m < 2 || RandomGeneratorIntegerRange(generator->random, 0, 3) == 0) && xx >= 0 && zz >= 0 && yy >= 1 && xx < w && zz < d && yy < heights[xx + zz * w] - 1 && generator->blocks[c] == 0) {
					if (generator->blocks[((yy - 1) * d + zz) * w + xx] == BlockTypeStone) {
						if (m == 0) { generator->blocks[c] = BlockTypeBrownMushroom; }
						else { generator->blocks[c] = BlockTypeRedMushroom; }
					}
				}
			}
		}
	}
	
	Level level = LevelCreate();
	level->waterLevel = generator->waterLevel;
	LevelSetData(level, w, 64, d, generator->blocks);
	ii = w * d / 4000;
	for (int i = 0; i < ii; i++) {
		ProgressBarDisplaySetProgress(generator->progressBar, i * 50 / (ii - 1) + 50);
		int x = (int)RandomGeneratorIntegerRange(generator->random, 0, w - 1);
		int z = (int)RandomGeneratorIntegerRange(generator->random, 0, d - 1);
		for (int j = 0; j < 20; j++) {
			int xx = x;
			int zz = z;
			for (int k = 0; k < 20; k++) {
				xx += (int)RandomGeneratorIntegerRange(generator->random, 0, 5) - (int)RandomGeneratorIntegerRange(generator->random, 0, 5);
				zz += (int)RandomGeneratorIntegerRange(generator->random, 0, 5) - (int)RandomGeneratorIntegerRange(generator->random, 0, 5);
				if (xx >= 0 && zz >= 0 && xx < w && zz < d) {
					int y = heights[xx + zz * w] + 1;
					if (RandomGeneratorIntegerRange(generator->random, 0, 3) == 0) { LevelMaybeGrowTree(level, xx, y, zz); }
				}
			}
		}
	}
	free(generator->blocks);
	free(heights);
	return level;
}

void LevelGeneratorDestroy(LevelGenerator generator) {
	RandomGeneratorDestroy(generator->random);
	free(generator->floodData);
	free(generator);
}
