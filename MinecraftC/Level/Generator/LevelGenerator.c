#include "LevelGenerator.h"
#include "Noise/Noise.h"
#include "Noise/OctaveNoise.h"
#include "Noise/CombinedNoise.h"
#include "../../Utilities/Time.h"
#include "../../Utilities/Log.h"
#include "../../Utilities/SinTable.h"

LevelGenerator LevelGeneratorCreate(ProgressBarDisplay progressBar)
{
	LevelGenerator generator = MemoryAllocate(sizeof(struct LevelGenerator));
	*generator = (struct LevelGenerator)
	{
		.ProgressBar = progressBar,
		.Random = RandomGeneratorCreate(time(NULL)),
		.FloodData = MemoryAllocate(1024 * 1024 * sizeof(int)),
	};
	return generator;
}

static void PopulateOre(LevelGenerator generator, BlockType ore, int a1, int a2, int a3)
{
	int w = generator->Width;
	int h = generator->Height;
	int d = generator->Depth;
	int ii = w * d * h / 256 / 64 * a1 / 100;
	for (int i = 0; i < ii; i++)
	{
		ProgressBarDisplaySetProgress(generator->ProgressBar, i * 100 / (ii - 1) / 4 + a2 * 100 / 4);
		float3 v1 = { RandomGeneratorUniform(generator->Random) * w, RandomGeneratorUniform(generator->Random) * h, RandomGeneratorUniform(generator->Random) * d };
		int jj = (RandomGeneratorUniform(generator->Random) + RandomGeneratorUniform(generator->Random)) * 75.0 * a1 / 100.0;
		float2 r1 = (float2){ RandomGeneratorUniform(generator->Random), RandomGeneratorUniform(generator->Random) } * 2.0 * pi;
		float2 r2 = { 0.0, 0.0 };
		for (int j = 0; j < jj; j++)
		{
			v1 += (float3){ tsin(r1.x) * tcos(r1.y), tsin(r1.y), tcos(r1.x) * tcos(r1.y) };
			r1.x += r2.x * 0.2;
			r2.x = r2.x * 0.9 + RandomGeneratorUniform(generator->Random) - RandomGeneratorUniform(generator->Random);
			r1.y = (r1.y + r2.y * 0.5) * 0.5;
			r2.y = r2.y * 0.9 + RandomGeneratorUniform(generator->Random) - RandomGeneratorUniform(generator->Random);
			float v2 = tsin(j * pi / jj) * a1 / 100.0 + 1.0;
			for (int x = v1.x - v2; x <= (int)(v1.x + v2); x++)
			{
				for (int y = v1.y - v2; y <= (int)(v1.y + v2); y++)
				{
					for (int z = v1.z - v2; z <= (int)(v1.z + v2); z++)
					{
						float3 v3 = (float3){ x, y, z } - v1;
						if (dot3f(v3, v3) + v3.y * v3.y < v2 * v2 && x >= 1 && y >= 1 && z >= 1 && x < w - 1 && y < h - 1 && z < d - 1)
						{
							int c = (y * d + z) * w + x;
							if (generator->Blocks[c] == BlockTypeStone) { generator->Blocks[c] = ore; }
						}
					}
				}
			}
		}
	}
}

static int64_t Flood(LevelGenerator generator, int x, int y, int z, int var, BlockType tile)
{
	list(int *) floodStack = ListCreate(sizeof(int *));
	
	int xx = 1, zz = 1;
	for (xx = 1; 1 << zz < generator->Width; zz++);
	while (1 << xx < generator->Depth) { xx++; }
	int d = generator->Depth - 1;
	int w = generator->Width - 1;
	generator->FloodData[0] = (((y << xx) + z) << zz) + x;
	int i = 1;
	int64_t j = 0;
	int ii = generator->Width * generator->Depth;
	while (i > 0)
	{
		i--;
		int f = generator->FloodData[i];
		if (i == 0 && ListCount(floodStack) > 0)
		{
			MemoryFree(generator->FloodData);
			generator->FloodData = floodStack[ListCount(floodStack) - 1];
			floodStack = ListRemove(floodStack, ListCount(floodStack) - 1);
		}
		int v1 = f >> zz & d;
		int v2 = f >> (zz + xx);
		int v3 = f & w, v4 = f & w;
		for (v3 = v3; v3 > 0 && generator->Blocks[f - 1] == 0; f--) { v3--; }
		while (v4 < generator->Width && generator->Blocks[f + v4 - v3] == 0) { v4++; }
		int v5 = f >> zz & d;
		int v6 = f >> (zz + xx);
		if (v5 != v1 || v6 != v2) { LogDebug("Diagonal flood!?\n"); }
		
		bool b1 = false, b2 = false, b3 = false;
		j += v4 - v3;
		for (v3 = v3; v3 < v4; v3++)
		{
			generator->Blocks[f] = tile;
			if (v1 > 0)
			{
				bool b4 = generator->Blocks[f - generator->Width] == 0;
				if (b4 && !b1)
				{
					if (i == 1024 * 1024)
					{
						floodStack = ListPush(floodStack, &generator->FloodData);
						generator->FloodData = MemoryAllocate(1024 * 1024 * sizeof(int));
						i = 0;
					}
					generator->FloodData[i++] = f - generator->Width;
				}
				b1 = b4;
			}
			if (v1 < generator->Depth - 1)
			{
				bool b4 = generator->Blocks[f + generator->Width] == 0;
				if (b4 && !b2)
				{
					if (i == 1024 * 1024)
					{
						floodStack = ListPush(floodStack, &generator->FloodData);
						generator->FloodData = MemoryAllocate(1024 * 1024 * sizeof(int));
						i = 0;
					}
					generator->FloodData[i++] = f + generator->Width;
				}
				b2 = b4;
			}
			if (v2 > 0)
			{
				BlockType t = generator->Blocks[f - ii];
				if ((tile == BlockTypeLava || tile == BlockTypeStillLava) && (t == BlockTypeWater || t == BlockTypeStillWater)) { generator->Blocks[f - ii] = BlockTypeStone; }
				bool b4 = t == 0;
				if (b4 && !b3)
				{
					if (i == 1024 * 1024)
					{
						floodStack = ListPush(floodStack, &generator->FloodData);
						generator->FloodData = MemoryAllocate(1024 * 1024 * sizeof(int));
						i = 0;
					}
					generator->FloodData[i++] = f - ii;
				}
				b3 = b4;
			}
			f++;
		}
	}
	for (int i = 0; i < ListCount(floodStack); i++) { MemoryFree(floodStack[i]); }
	ListDestroy(floodStack);
	return j;
}

Level LevelGeneratorGenerate(LevelGenerator generator, const char * userName, int width, int depth, int var)
{
	ProgressBarDisplaySetTitle(generator->ProgressBar, "Generating level");
	generator->Width = width;
	generator->Depth = depth;
	generator->Height = 64;
	generator->WaterLevel = 32;
	generator->Blocks = MemoryAllocate(width * depth << 6);
	int w = generator->Width;
	int h = generator->Height;
	int d = generator->Depth;
	
	ProgressBarDisplaySetText(generator->ProgressBar, "Raising..");
	CombinedNoise n1 = CombinedNoiseCreate(OctaveNoiseCreate(generator->Random, 8), OctaveNoiseCreate(generator->Random, 8));
	CombinedNoise n2 = CombinedNoiseCreate(OctaveNoiseCreate(generator->Random, 8), OctaveNoiseCreate(generator->Random, 8));
	OctaveNoise n3 = OctaveNoiseCreate(generator->Random, 6);
	int * heights = MemoryAllocate(w * d * sizeof(int));
	for (int x = 0; x < w; x++)
	{
		ProgressBarDisplaySetProgress(generator->ProgressBar, x * 100 / (w - 1));
		for (int y = 0; y < d; y++)
		{
 			float v1 = NoiseCompute(n1, (float2){ x, y } * 1.3) / 6.0 - 4.0;
			float v2 = NoiseCompute(n2, (float2){ x, y } * 1.3) / 5.0 + 6.0;
			if (NoiseCompute(n3, (float2){ x, y }) / 8.0 > 0.0) { v2 = v1; }
			float m = fmaxf(v1, v2) / 2.0;
			if (m < 0.0) { m *= 0.8; }
			heights[x + y * w] = m;
		}
	}
	NoiseDestroy(((CombinedNoiseData)n1->TypeData)->Noise1);
	NoiseDestroy(((CombinedNoiseData)n1->TypeData)->Noise2);
	NoiseDestroy(n1);
	NoiseDestroy(((CombinedNoiseData)n2->TypeData)->Noise1);
	NoiseDestroy(((CombinedNoiseData)n2->TypeData)->Noise2);
	NoiseDestroy(n2);
	NoiseDestroy(n3);
	
	ProgressBarDisplaySetText(generator->ProgressBar, "Eroding..");
	n1 = CombinedNoiseCreate(OctaveNoiseCreate(generator->Random, 8), OctaveNoiseCreate(generator->Random, 8));
	n2 = CombinedNoiseCreate(OctaveNoiseCreate(generator->Random, 8), OctaveNoiseCreate(generator->Random, 8));
	for (int x = 0; x < w; x++)
	{
		ProgressBarDisplaySetProgress(generator->ProgressBar, x * 100 / (w - 1));
		for (int y = 0; y < d; y++)
		{
			float v1 = NoiseCompute(n1, (float2){ x, y } * 2.0) / 8.0;
			int v2 = NoiseCompute(n2, (float2){ x, y } * 2.0) > 0.0 ? 1 : 0;
			if (v1 > 2.0) { heights[x + y * w] = ((heights[x + y * w] - v2) / 2 << 1) + v2; }
		}
	}
	NoiseDestroy(((CombinedNoiseData)n1->TypeData)->Noise1);
	NoiseDestroy(((CombinedNoiseData)n1->TypeData)->Noise2);
	NoiseDestroy(n1);
	NoiseDestroy(((CombinedNoiseData)n2->TypeData)->Noise1);
	NoiseDestroy(((CombinedNoiseData)n2->TypeData)->Noise2);
	NoiseDestroy(n2);
	
	ProgressBarDisplaySetText(generator->ProgressBar, "Soiling..");
	OctaveNoise n = OctaveNoiseCreate(generator->Random, 6);
	for (int x = 0; x < w; x++)
	{
		ProgressBarDisplaySetProgress(generator->ProgressBar, x * 100 / (w - 1));
		for (int y = 0; y < d; y++)
		{
			int v1 = (int)(NoiseCompute(n, (float2){ x, y }) / 24.0) - 4;
			int v2 = heights[x + y * w] + generator->WaterLevel;
			int v3 = v2 + v1;
			heights[x + y * w] = v2 > v3 ? v2 : v3;
			if (heights[x + y * w] > h - 2) { heights[x + y * w] = h - 2; }
			if (heights[x + y * w] < 1) { heights[x + y * w] = 1; }
			for (int z = 0; z < h; z++)
			{
				int c = (z * d + y) * w + x;
				BlockType tile = BlockTypeNone;
				if (z <= v2) { tile = BlockTypeDirt; }
				if (z <= v3) { tile = BlockTypeStone; }
				if (z == 0) { tile = BlockTypeLava; }
				generator->Blocks[c] = tile;
			}
		}
	}
	NoiseDestroy(n);
	
	ProgressBarDisplaySetText(generator->ProgressBar, "Carving..");
	int ii = w * d * h / 256 / 64 << 1;
	for (int i = 0; i < ii; i++)
	{
		ProgressBarDisplaySetProgress(generator->ProgressBar, i * 100 / (ii - 1));
		float3 v1 = { RandomGeneratorUniform(generator->Random) * w, RandomGeneratorUniform(generator->Random) * h, RandomGeneratorUniform(generator->Random) * d };
		int jj = (RandomGeneratorUniform(generator->Random) + RandomGeneratorUniform(generator->Random)) * 200.0;
		float2 r1 = (float2){ RandomGeneratorUniform(generator->Random), RandomGeneratorUniform(generator->Random) } * 2.0 * pi;
		float2 r2 = { 0.0, 0.0 };
		float v2 = RandomGeneratorUniform(generator->Random) * RandomGeneratorUniform(generator->Random);
		for (int j = 0; j < jj; j++)
		{
			v1 += (float3){ tsin(r1.x) * tcos(r1.y), tsin(r1.y), tcos(r1.x) * tcos(r1.y) };
			r1.x = (r1.x + r1.x * 0.2) * 0.9;
			r2.x = r1.x + RandomGeneratorUniform(generator->Random) - RandomGeneratorUniform(generator->Random);
			r1.y = (r1.y + r2.y * 0.5) * 0.5;
			r2.y = r2.y * 0.75 + RandomGeneratorUniform(generator->Random) - RandomGeneratorUniform(generator->Random);
			if (RandomGeneratorUniform(generator->Random) >= 0.25)
			{
				float3 v3 = v1 + ((float3){ RandomGeneratorUniform(generator->Random), RandomGeneratorUniform(generator->Random), RandomGeneratorUniform(generator->Random) } * 4.0 - 2.0) * 0.2;
				float v4 = (h - v3.y) / h;
				v4 = 1.2 + (v4 * 3.5 + 1.0) * v2;
				v4 *= tsin(j * pi / jj);
				for (int x = v3.x - v4; x <= (int)(v3.x + v4); x++)
				{
					for (int y = v3.y - v4; y <= (int)(v3.y + v4); y++)
					{
						for (int z = v3.z - v4; z <= (int)(v3.z + v4); z++)
						{
							float3 v5 = (float3){ x, y, z } - v3;
							if (dot3f(v5, v5) + v5.y * v5.y < v4 * v4 && x >= 1 && y >= 1 && z >= 1 && x < w - 1 && y < h - 1 && z < d - 1)
							{
								int c = (y * d + z) * w + x;
								if (generator->Blocks[c] == BlockTypeStone) { generator->Blocks[c] = BlockTypeNone; }
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
	
	ProgressBarDisplaySetText(generator->ProgressBar, "Watering..");
	ProgressBarDisplaySetProgress(generator->ProgressBar, 0);
	BlockType flood = BlockTypeStillWater;
	for (int x = 0; x < w; x++)
	{
		Flood(generator, x, h / 2 - 1, 0, 0, flood);
		Flood(generator, x, h / 2 - 1, d - 1, 0, flood);
	}
	for (int z = 0; z < d; z++)
	{
		Flood(generator, 0, h / 2 - 1, z, 0, flood);
		Flood(generator, w - 1, h / 2 - 1, z, 0, flood);
	}
	ii = w * d / 8000;
	for (int i = 0; i < ii; i++)
	{
		if (i % 100 == 0) { ProgressBarDisplaySetProgress(generator->ProgressBar, i * 100 / (ii - 1)); }
		int x = (int)RandomGeneratorIntegerRange(generator->Random, 0, w - 1);
		int y = generator->WaterLevel - 1 - (int)RandomGeneratorIntegerRange(generator->Random, 0, 1);
		int z = (int)RandomGeneratorIntegerRange(generator->Random, 0, d - 1);
		if (generator->Blocks[(y * d + z) * w + x] == BlockTypeNone) { Flood(generator, x, y, z, 0, flood); }
	}
	ProgressBarDisplaySetProgress(generator->ProgressBar, 100);
	
	ProgressBarDisplaySetText(generator->ProgressBar, "Melting..");
	ii = w * d * h / 20000;
	for (int i = 0; i < ii; i++)
	{
		if (i % 100 == 0) { ProgressBarDisplaySetProgress(generator->ProgressBar, i * 100 / (ii - 1)); }
		int x = (int)RandomGeneratorIntegerRange(generator->Random, 0, w - 1);
		int y = RandomGeneratorUniform(generator->Random) * RandomGeneratorUniform(generator->Random) * (generator->WaterLevel - 3);
		int z = (int)RandomGeneratorIntegerRange(generator->Random, 0, d - 1);
		if (generator->Blocks[(y * d + z) * w + x] == BlockTypeNone) { Flood(generator, x, y, z, 0, BlockTypeStillLava); }
	}
	ProgressBarDisplaySetProgress(generator->ProgressBar, 100);
	
	ProgressBarDisplaySetText(generator->ProgressBar, "Growing..");
	n1 = OctaveNoiseCreate(generator->Random, 8);
	n2 = OctaveNoiseCreate(generator->Random, 8);
	for (int x = 0; x < w; x++)
	{
		ProgressBarDisplaySetProgress(generator->ProgressBar, x * 100 / (w - 1));
		for (int y = 0; y < d; y++)
		{
			bool v1 = NoiseCompute(n1, (float2){ x, y }) > 8.0;
			bool v2 = NoiseCompute(n2, (float2){ x, y }) > 12.0;
			int z = heights[x + y * w];
			int c = (z * d + y) * w + x;
			BlockType above = generator->Blocks[((z + 1) * d + y) * w + x];
			if ((above == BlockTypeWater || above == BlockTypeStillWater) && z <= h / 2 - 1 && v2) { generator->Blocks[c] = BlockTypeGravel; }
			if (above == BlockTypeNone)
			{
				BlockType tile = BlockTypeGrass;
				if (z <= h / 2 - 1 && v1) { tile = BlockTypeSand; }
				generator->Blocks[c] = tile;
			}
		}
	}
	NoiseDestroy(n1);
	NoiseDestroy(n2);
	
	ProgressBarDisplaySetText(generator->ProgressBar, "Planting..");
	ii = w * d / 3000;
	for (int i = 0; i < ii; i++)
	{
		ProgressBarDisplaySetProgress(generator->ProgressBar, i * 50 / (ii - 1));
		int x = (int)RandomGeneratorIntegerRange(generator->Random, 0, w - 1);
		int z = (int)RandomGeneratorIntegerRange(generator->Random, 0, d - 1);
		int f = (int)RandomGeneratorIntegerRange(generator->Random, 0, 1);
		for (int j = 0; j < 10; j++)
		{
			int xx = x;
			int zz = z;
			for (int k = 0; k < 5; k++)
			{
				xx += (int)RandomGeneratorIntegerRange(generator->Random, 0, 5) - (int)RandomGeneratorIntegerRange(generator->Random, 0, 5);
				zz += (int)RandomGeneratorIntegerRange(generator->Random, 0, 5) - (int)RandomGeneratorIntegerRange(generator->Random, 0, 5);
				if ((f < 2 || RandomGeneratorIntegerRange(generator->Random, 0, 3) == 0) && xx >= 0 && zz >= 0 && xx < w && zz < d)
				{
					int y = heights[xx + zz * width] + 1;
					int c = (y * d + zz) * w + xx;
					if (generator->Blocks[c] == BlockTypeNone)
					{
						if (generator->Blocks[((y - 1) * d + zz) * w + xx] == BlockTypeGrass)
						{
							if (f == 0) { generator->Blocks[c] = BlockTypeDandelion; }
							else if (f == 1) { generator->Blocks[c] = BlockTypeRose; }
						}
					}
				}
			}
		}
	}
	ii = w * d * h / 2000;
	for (int i = 0; i < ii; i++)
	{
		ProgressBarDisplaySetProgress(generator->ProgressBar, i * 50 / (ii - 1) + 50);
		int m = (int)RandomGeneratorIntegerRange(generator->Random, 0, 1);
		int x = (int)RandomGeneratorIntegerRange(generator->Random, 0, w - 1);
		int y = (int)RandomGeneratorIntegerRange(generator->Random, 0, h - 1);
		int z = (int)RandomGeneratorIntegerRange(generator->Random, 0, d - 1);
		for (int j = 0; j < 20; j++)
		{
			int xx = x;
			int yy = y;
			int zz = z;
			for (int k = 0; k < 5; k++)
			{
				xx += (int)RandomGeneratorIntegerRange(generator->Random, 0, 5) - (int)RandomGeneratorIntegerRange(generator->Random, 0, 5);
				yy += (int)RandomGeneratorIntegerRange(generator->Random, 0, 1) - (int)RandomGeneratorIntegerRange(generator->Random, 0, 1);
				zz += (int)RandomGeneratorIntegerRange(generator->Random, 0, 5) - (int)RandomGeneratorIntegerRange(generator->Random, 0, 5);
				int c = (yy * d + zz) * w + xx;
				if ((m < 2 || RandomGeneratorIntegerRange(generator->Random, 0, 3) == 0) && xx >= 0 && zz >= 0 && yy >= 1 && xx < w && zz < d && yy < heights[xx + zz * w] - 1 && generator->Blocks[c] == 0)
				{
					if (generator->Blocks[((yy - 1) * d + zz) * w + xx] == BlockTypeStone)
					{
						if (m == 0) { generator->Blocks[c] = BlockTypeBrownMushroom; }
						else { generator->Blocks[c] = BlockTypeRedMushroom; }
					}
				}
			}
		}
	}
	
	Level level = LevelCreate();
	level->WaterLevel = generator->WaterLevel;
	LevelSetData(level, w, 64, d, generator->Blocks);
	level->CreateTime = TimeMilli();
	level->Creator = userName;
	level->Name = "A Nice World";
	ii = w * d / 4000;
	for (int i = 0; i < ii; i++)
	{
		ProgressBarDisplaySetProgress(generator->ProgressBar, i * 50 / (ii - 1) + 50);
		int x = (int)RandomGeneratorIntegerRange(generator->Random, 0, w - 1);
		int z = (int)RandomGeneratorIntegerRange(generator->Random, 0, d - 1);
		for (int j = 0; j < 20; j++)
		{
			int xx = x;
			int zz = z;
			for (int k = 0; k < 20; k++)
			{
				xx += (int)RandomGeneratorIntegerRange(generator->Random, 0, 5) - (int)RandomGeneratorIntegerRange(generator->Random, 0, 5);
				zz += (int)RandomGeneratorIntegerRange(generator->Random, 0, 5) - (int)RandomGeneratorIntegerRange(generator->Random, 0, 5);
				if (xx >= 0 && zz >= 0 && xx < w && zz < d)
				{
					int y = heights[xx + zz * w] + 1;
					if (RandomGeneratorIntegerRange(generator->Random, 0, 3) == 0) { LevelMaybeGrowTree(level, xx, y, zz); }
				}
			}
		}
	}
	MemoryFree(generator->Blocks);
	MemoryFree(heights);
	return level;
}

void LevelGeneratorDestroy(LevelGenerator generator)
{
	RandomGeneratorDestroy(generator->Random);
	MemoryFree(generator->FloodData);
	MemoryFree(generator);
}
