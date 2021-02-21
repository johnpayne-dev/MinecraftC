#include <string.h>
#include "Level.h"
#include "../Render/LevelRenderer.h"
#include "../Utilities/Log.h"

Level LevelCreate()
{
	Level level = MemoryAllocate(sizeof(struct Level));
	*level = (struct Level)
	{
		.Renderers = ListCreate(sizeof(LevelRenderer)),
		.Random = RandomGeneratorCreate(TimeNano()),
		.TickList = ListCreate(sizeof(NextTickListEntry)),
		.NetworkMode = false,
		.Unprocessed = 0,
		.TickCount = 0,
		.GrowTrees = false,
	};
	level->RandomValue = (int)RandomGeneratorInteger(level->Random);
	return level;
}

void LevelInitializeTransient(Level level)
{
	if (level->Blocks == NULL) { LogFatal("The level is corrupt!\n"); }
	//level->Renderers = ListCreate(sizeof(LevelRenderer));
	//level->LightBlockers = MemoryAllocate(level->Width * level->Height * sizeof(int));
	//for (int i = 0; i < level->Width * level->Height; i++) { level->LightBlockers[i] = level->Depth; }
	//LevelCalculateLightDepths(level, 0, 0, level->Width, level->Height);
	//level->Random = RandomGeneratorCreate(0);
	//level->RandomValue = (int)RandomGeneratorInteger(level->Random);
	level->SkyColor = ColorFromHex(0x99CCFFFF);
	level->FogColor = ColorWhite;
	level->CloudColor = ColorWhite;
	LevelFindSpawn(level);
	if (level->BlockMap == NULL) { level->BlockMap = BlockMapCreate(level->Width, level->Depth, level->Height); }
}

void LevelSetData(Level level, int w, int d, int h, unsigned char * blocks)
{
	level->Width = w;
	level->Depth = d;
	level->Height = h;
	level->Blocks = MemoryAllocate(w * d * h);
	memcpy(level->Blocks, blocks, w * d * h);
	level->LightBlockers = MemoryAllocate(w * h * sizeof(int));
	for (int i = 0; i < w * h; i++) { level->LightBlockers[i] = level->Depth - 1; }
	LevelCalculateLightDepths(level, 0, 0, w, h);
	for (int i = 0; i < ListCount(level->Renderers); i++) { LevelRendererRefresh(level->Renderers[i]); }
	level->TickList = ListClear(level->TickList);
	LevelFindSpawn(level);
	LevelInitializeTransient(level);
}

void LevelFindSpawn(Level level)
{
	int i = 0;
	int3 v = { 0, 0, 0 };
	do
	{
		i++;
		v.xz = (int2){ RandomIntegerRange(level->Width / 4, level->Width / 2), RandomIntegerRange(level->Height / 4, level->Height / 2) };
		v.y = LevelGetHighestTile(level, v.x, v.z) + 1;
		if (i == 10000) { level->Spawn = (int3){ v.x, -100, v.z }; return; }
	} while (v.y <= LevelGetWaterLevel(level));
	level->Spawn = v;
	return;
}

void LevelCalculateLightDepths(Level level, int x0, int y0, int x1, int y1)
{
	for (int i = x0; i < x0 + x1; i++)
	{
		for (int j = y0; j < y0 + y1; j++)
		{
			int blocker = level->LightBlockers[i + j * level->Width];
			int k = level->Depth - 1;
			while (!LevelIsLightBlocker(level, i, k, j)) { k--; }
			level->LightBlockers[i + j * level->Width] = k;
			if (blocker != k)
			{
				int min = blocker < k ? blocker : k;
				int max = blocker > k ? blocker : k;
				for (int l = 0; l < ListCount(level->Renderers); l++) { LevelRendererQueueChunks(level->Renderers[l], (int3){ i, min, j } - 1, (int3){ i, max, j } + 1); }
			}
		}
	}
}

void LevelAddRenderer(Level level, LevelRenderer listener)
{
	level->Renderers = ListPush(level->Renderers, &listener);
}

void LevelFinalize(Level level)
{
	
}

void LevelRemoveRenderer(Level level, LevelRenderer listener)
{
	level->Renderers = ListRemoveAll(level->Renderers, &listener);
}

bool LevelIsLightBlocker(Level level, int x, int y, int z)
{
	Block block = Blocks.Table[LevelGetTile(level, x, y, z)];
	return block == NULL ? false : BlockIsOpaque(block);
}

list(AABB) LevelGetCubes(Level level, AABB box)
{
	list(AABB) list = ListCreate(sizeof(AABB));
	int3 v0 = int3f(box.V0);
	int3 v1 = int3f(box.V1) + 1;
	if (box.V0.x < 0.0) { v0.x--; }
	if (box.V0.y < 0.0) { v0.y--; }
	if (box.V0.z < 0.0) { v0.z--; }
	for (int i = v0.x; i < v1.x; i++)
	{
		for (int j = v0.y; j < v1.y; j++)
		{
			for (int k = v0.z; k < v1.z; k++)
			{
				AABB aabb = { 0 };
				if (i >= 0 && j >= 0 && k >= 0 && i < level->Width && j < level->Depth && k < level->Height)
				{
					Block block = Blocks.Table[LevelGetTile(level, i, j, k)];
					if (block != NULL)
					{
						aabb = BlockGetCollisionAABB(block, i, j, k);
						if (!AABBIsNull(aabb) && AABBIntersectsInner(box, aabb)) { list = ListPush(list, &aabb); }
					}
				}
				else if (i < 0 || j < 0 || k < 0 || i >= level->Width || k >= level->Height)
				{
					AABB aabb = BlockGetCollisionAABB(Blocks.Table[BlockTypeBedrock], i, j, k);
					if (!AABBIsNull(aabb) && AABBIntersectsInner(box, aabb)) { list = ListPush(list, &aabb); }
				}
			}
		}
	}
	return list;
}

void LevelSwap(Level level, int3 a, int3 b)
{
	if (!level->NetworkMode)
	{
		BlockType t1 = LevelGetTile(level, a.x, a.y, a.z);
		BlockType t2 = LevelGetTile(level, b.x, b.y, b.z);
		LevelSetTileNoNeighborChange(level, a.x, a.y, a.z, t2);
		LevelSetTileNoNeighborChange(level, b.x, b.y, b.z, t1);
		LevelUpdateNeighborsAt(level, a.x, a.y, a.z, t2);
		LevelUpdateNeighborsAt(level, b.x, b.y, b.z, t1);
	}
}

bool LevelSetTile(Level level, int x, int y, int z, BlockType tile)
{
	if (level->NetworkMode) { return false; }
	else if (LevelSetTileNoNeighborChange(level, x, y, z, tile))
	{
		LevelUpdateNeighborsAt(level, x, y, z, tile);
		return true;
	}
	else { return false; }
}

bool LevelNetSetTile(Level level, int x, int y, int z, BlockType tile)
{
	if (LevelNetSetTileNoNeighborChange(level, x, y, z, tile))
	{
		LevelUpdateNeighborsAt(level, x, y, z, tile);
		return true;
	}
	else { return false; }
}

bool LevelSetTileNoNeighborChange(Level level, int x, int y, int z, BlockType tile)
{
	return level->NetworkMode ? false : LevelNetSetTileNoNeighborChange(level, x, y, z, tile);
}

bool LevelNetSetTileNoNeighborChange(Level level, int x, int y, int z, BlockType tile)
{
	if (x < 0 || y < 0 || z < 0 || x >= level->Width || y >= level->Depth || z >= level->Height) { return false; }
	int i = (y * level->Height + z) * level->Width + x;
	if (tile == level->Blocks[i]) { return false; }
	
	if (tile == BlockTypeNone && (x == 0 || z == 0 || x == level->Width - 1 || z == level->Height - 1) && y >= LevelGetGroundLevel(level) && y < LevelGetWaterLevel(level)) { tile = BlockTypeWater; }
	BlockType prev = level->Blocks[i];
	level->Blocks[i] = tile;
	if (prev != BlockTypeNone) { BlockOnRemoved(Blocks.Table[prev], level, x, y, z); }
	if (tile != BlockTypeNone) { BlockOnAdded(Blocks.Table[tile], level, x, y, z); }
	LevelCalculateLightDepths(level, x, z, 1, 1);
	for (int j = 0; j < ListCount(level->Renderers); j++)
	{
		LevelRendererQueueChunks(level->Renderers[j], (int3){ x, y, z } - 1, (int3){ x, y, z } + 1);
	}
	return true;
}

static void UpdateTile(Level level, int x, int y, int z, BlockType tile)
{
	if (x < 0 || y < 0 || z < 0 || x >= level->Width || y >= level->Depth || z >= level->Height) { return; }
	Block block = Blocks.Table[level->Blocks[(y * level->Height + z) * level->Width + x]];
	if (block != NULL) { BlockOnNeighborChanged(block, level, x, y, z, tile); }
}

void LevelUpdateNeighborsAt(Level level, int x, int y, int z, BlockType tile)
{
	UpdateTile(level, x - 1, y, z, tile);
	UpdateTile(level, x + 1, y, z, tile);
	UpdateTile(level, x, y - 1, z, tile);
	UpdateTile(level, x, y + 1, z, tile);
	UpdateTile(level, x, y, z - 1, tile);
	UpdateTile(level, x, y, z + 1, tile);
}

bool LevelSetTileNoUpdate(Level level, int x, int y, int z, BlockType tile)
{
	if (x < 0 || y < 0 || z < 0 || x >= level->Width || y >= level->Depth || z >= level->Height) { return false; }
	int i = (y * level->Height + z) * level->Width + x;
	if (tile == level->Blocks[i]) { return false; }
	level->Blocks[i] = tile;
	return true;
}

bool LevelIsLit(Level level, int x, int y, int z)
{
	return !(x < 0 || y < 0 || z < 0 || x >= level->Width || y >= level->Depth || z >= level->Height) ? y >= level->LightBlockers[x + z * level->Width] : true;
}

BlockType LevelGetTile(Level level, int x, int y, int z)
{
	return !(x < 0 || y < 0 || z < 0 || x >= level->Width || y >= level->Depth || z >= level->Height) ? level->Blocks[(y * level->Height + z) * level->Width + x] & 0xFF : 0;
}

bool LevelIsSolidTile(Level level, int x, int y, int z)
{
	Block block = Blocks.Table[LevelGetTile(level, x, y, z)];
	return block == NULL ? false : BlockIsSolid(block);
}

void LevelTickEntities(Level level)
{
	BlockMapTickAll(level->BlockMap);
}

void LevelTick(Level level)
{
	level->TickCount++;
	
	int a = 1;
	int b = 1;
	for (b = 1; 1 << a < level->Width; a++);
	while (1 << b < level->Height) { b++; }
	
	if (level->TickCount % 5 == 0)
	{
		for (int i = 0; i < ListCount(level->TickList); i++)
		{
			NextTickListEntry nextTick = level->TickList[0];
			level->TickList = ListRemove(level->TickList, 0);
			if (nextTick.Ticks > 0)
			{
				nextTick.Ticks--;
				level->TickList = ListPush(level->TickList, &nextTick);
			}
			else
			{
				BlockType tile = level->Blocks[(nextTick.Position.y * level->Height + nextTick.Position.z) * level->Width + nextTick.Position.x];
				if (LevelIsInBounds(level, nextTick.Position.x, nextTick.Position.y, nextTick.Position.z) && tile == nextTick.Tile && tile != BlockTypeNone)
				{
					BlockUpdate(Blocks.Table[tile], level, nextTick.Position.x, nextTick.Position.y, nextTick.Position.z, level->Random);
				}
			}
		}
	}
	
	level->Unprocessed += level->Width * level->Height * level->Depth;
	int c = level->Unprocessed / 200;
	level->Unprocessed -= c * 200;
	
	for (int i = 0; i < c; i++)
	{
		level->RandomValue = level->RandomValue * 3 + 1013904223;
		int y = level->RandomValue >> 2;
		int x = y & (level->Width - 1);
		int z = y >> a & (level->Height - 1);
		y = y >> (a + b) & (level->Depth - 1);
		BlockType tile = level->Blocks[(y * level->Height + z) * level->Width + x];
		if (Blocks.Physics[tile]) { BlockUpdate(Blocks.Table[tile], level, x, y, z, level->Random); }
	}
}

int LevelCountInstancesOf(Level level, EntityType type)
{
	int count = 0;
	for (int i = 0; i < ListCount(level->BlockMap->All); i++)
	{
		if (level->BlockMap->All[i]->Type == type) { count++; }
	}
	return count;
}

bool LevelIsInBounds(Level level, int x, int y, int z)
{
	return !(x < 0 || y < 0 || z < 0 || x >= level->Width || y >= level->Depth || z >= level->Height);
}

float LevelGetGroundLevel(Level level)
{
	return LevelGetWaterLevel(level) - 2.0;
}

float LevelGetWaterLevel(Level level)
{
	return level->WaterLevel;
}

bool LevelContainsAnyLiquid(Level level, AABB box)
{
	int3 v0 = int3f(box.V0);
	int3 v1 = int3f(box.V1) + 1;
	if (box.V0.x < 0.0) { v0.x--; }
	if (box.V0.y < 0.0) { v0.y--; }
	if (box.V0.z < 0.0) { v0.z--; }
	if (v0.x < 0) { v0.x = 0; }
	if (v0.y < 0) { v0.y = 0; }
	if (v0.z < 0) { v0.z = 0; }
	if (v1.x > level->Width) { v1.x = level->Width; }
	if (v1.y > level->Depth) { v1.y = level->Depth; }
	if (v1.z > level->Height) { v1.z = level->Height; }
	for (int i = v0.x; i < v1.x; i++)
	{
		for (int j = v0.y; j < v1.y; j++)
		{
			for (int k = v0.z; k < v1.z; k++)
			{
				Block block = Blocks.Table[LevelGetTile(level, i, j, k)];
				if (block != NULL && BlockGetLiquidType(block) != LiquidTypeNone) { return true; }
			}
		}
	}
	return false;
}

bool LevelContainsLiquid(Level level, AABB box, LiquidType liquidID)
{
	int3 v0 = int3f(box.V0);
	int3 v1 = int3f(box.V1) + 1;
	if (box.V0.x < 0.0) { v0.x--; }
	if (box.V0.y < 0.0) { v0.y--; }
	if (box.V0.z < 0.0) { v0.z--; }
	if (v0.x < 0) { v0.x = 0; }
	if (v0.y < 0) { v0.y = 0; }
	if (v0.z < 0) { v0.z = 0; }
	if (v1.x > level->Width) { v1.x = level->Width; }
	if (v1.y > level->Depth) { v1.y = level->Depth; }
	if (v1.z > level->Height) { v1.z = level->Height; }
	for (int i = v0.x; i < v1.x; i++)
	{
		for (int j = v0.y; j < v1.y; j++)
		{
			for (int k = v0.z; k < v1.z; k++)
			{
				Block block = Blocks.Table[LevelGetTile(level, i, j, k)];
				if (block != NULL && BlockGetLiquidType(block) == liquidID) { return true; }
			}
		}
	}
	return false;
}

void LevelAddToNextTick(Level level, int x, int y, int z, BlockType tile)
{
	if (level->NetworkMode) { return; }
	
	NextTickListEntry tick = { .Position = { x, y, z }, .Tile = tile };
	if (tile != BlockTypeNone) { tick.Ticks = BlockGetTickDelay(Blocks.Table[tile]); }
	level->TickList = ListPush(level->TickList, &tick);
}

bool LevelIsFree(Level level, AABB aabb)
{
	list(Entity) entities = BlockMapGetEntitiesAABB(level->BlockMap, NULL, aabb);
	int count = ListCount(entities);
	ListDestroy(entities);
	return count == 0;
}

list(Entity) LevelFindEntities(Level level, AABB aabb)
{
	return BlockMapGetEntitiesAABB(level->BlockMap, NULL, aabb);
}

static bool IsSolid(Level level, float x, float y, float z)
{
	BlockType tile = LevelGetTile(level, x, y, z);
	return tile != BlockTypeNone && BlockIsSolid(Blocks.Table[tile]);
}

bool LevelIsSolidSearch(Level level, float3 v, float r)
{
	if (IsSolid(level, v.x - r, v.y - r, v.z - r)) { return true; }
	if (IsSolid(level, v.x + r, v.y - r, v.z - r)) { return true; }
	if (IsSolid(level, v.x - r, v.y + r, v.z - r)) { return true; }
	if (IsSolid(level, v.x + r, v.y + r, v.z - r)) { return true; }
	if (IsSolid(level, v.x - r, v.y - r, v.z + r)) { return true; }
	if (IsSolid(level, v.x + r, v.y - r, v.z + r)) { return true; }
	if (IsSolid(level, v.x - r, v.y + r, v.z + r)) { return true; }
	if (IsSolid(level, v.x + r, v.y + r, v.z + r)) { return true; }
	return false;
}

int LevelGetHighestTile(Level level, int x, int z)
{
	int y;
	for (y = level->Depth; (LevelGetTile(level, x, y - 1, z) == 0 || BlockGetLiquidType(Blocks.Table[LevelGetTile(level, x, y - 1, z)]) != LiquidTypeNone) && y > 0; y--);
	return y;
}

void LevelSetSpawnPosition(Level level, int x, int y, int z, float rotation)
{
	level->Spawn = (int3){ x, y, z };
	level->SpawnRotation = rotation;
}

float LevelGetBrightness(Level level, int x, int y, int z)
{
	return LevelIsLit(level, x, y, z) ? 1.0 : 0.6;
}

float LevelGetCaveness(Level level, float3 pos, float degrees)
{
	int3 v = int3f(pos);
	float n = 0.0, m = 0.0;
	for (int x = v.x - 6; x <= v.x + 6; x++)
	{
		for (int z = v.z - 6; z <= v.z + 6; z++)
		{
			if (LevelIsInBounds(level, x, v.y, z) && !LevelIsSolidTile(level, x, v.y, z))
			{
				float ax = x + 0.5 - pos.x;
				float az = z + 0.5 - pos.z;
				float a;
				for (a = atan2(az, ax) - degrees * rad + pi / 2.0; a < -pi; a += 2.0 * pi);
				while (a >= pi) { a -= 2.0 * pi; }
				if (a < 0.0) { a = -a; }
				float l = 1.0 / length3f((float3){ ax, 2.0, az });
				if (a > 1.0) { l = 0.0; }
				if (l < 0.0) { l = 0.0; }
				m += l;
				if (LevelIsLit(level, x, v.y, z)) { n += l; }
			}
		}
	}
	if (m == 0.0) { return 0.0; }
	else { return n / m; }
}

float LevelGetCavenessEntity(Level level, Entity entity)
{
	float2 r1 = { cos(-entity->Rotation.y * deg + pi), sin(-entity->Rotation.y * deg + pi) };
	float2 r2 = { cos(-entity->Rotation.x * deg), sin(-entity->Rotation.x * deg) };
	float3 p = entity->Position;
	float f = 0.0, g = 0.0;
	for (int i = 0; i <= 200; i++)
	{
		float a = (i / 200.0 - 0.5) * 2.0;
		int j = 0;
		while (j <= 200)
		{
			float b = (j / 200.0 - 0.5) * 1.6;
			float c = r2.x * b + r2.y;
			b = r2.x - r2.y * b;
			float d = r1.x * a + r1.y * b;
			d = r1.x * b - r1.y * a;
			int k = 0;
			while (true)
			{
				if (k < 10)
				{
					float3 q = p + (float3){ d, c, b } * 0.8 * k;
					if (!IsSolid(level, q.x, q.y, q.z))
					{
						f++;
						if (LevelIsLit(level, q.x, q.y, q.z)) { g++; }
					}
					k++;
					continue;;
				}
				j++;
				break;
			}
		}
	}
	
	if (f == 0.0) { return 0.0; }
	else
	{
		float h = g / f / 0.1;
		if (h > 1.0) { h = 1.0; }
		h = 1.0 - h;
		return 1.0 - h * h * h;
	}
}

unsigned char * LevelCopyBlocks(Level level)
{
	unsigned char * blocks = MemoryAllocate(level->Width * level->Depth * level->Height);
	memcpy(blocks, level->Blocks, level->Width * level->Depth * level->Height);
	return blocks;
}

LiquidType LevelGetLiquidType(Level level, int x, int y, int z)
{
	BlockType tile = LevelGetTile(level, x, y, z);
	return tile == BlockTypeNone ? LiquidTypeNone : BlockGetLiquidType(Blocks.Table[tile]);
}

bool LevelIsWater(Level level, int x, int y, int z)
{
	BlockType tile = LevelGetTile(level, x, y, z);
	return tile != BlockTypeNone && BlockGetLiquidType(Blocks.Table[tile]) == LiquidTypeWater;
}

void LevelSetNetworkMode(Level level, bool network)
{
	level->NetworkMode = network;
}

MovingObjectPosition LevelClip(Level level, float3 v0, float3 v1)
{
	if (v0.x == NAN || v0.y == NAN || v0.z == NAN) { return (MovingObjectPosition){ .Null = true }; }
	if (v1.x == NAN || v1.y == NAN || v1.z == NAN) { return (MovingObjectPosition){ .Null = true }; }
	int3 i0 = int3f(v0);
	int3 i1 = int3f(v1);
	
	int i = 20;
	while (i-- >= 0)
	{
		if (v0.x == NAN || v0.y == NAN || v0.z == NAN) { return (MovingObjectPosition){ .Null = true }; }
		if (i0.x == i1.x && i0.y == i1.y && i0.z == i1.z) { return (MovingObjectPosition){ .Null = true }; }
		float3 a = one3f * 999.0;
		if (i1.x > i0.x) { a.x = i0.x + 1.0; }
		if (i1.x < i0.x) { a.x = i0.x; }
		if (i1.y > i0.y) { a.y = i0.y + 1.0; }
		if (i1.y < i0.y) { a.y = i0.y; }
		if (i1.z > i0.z) { a.z = i0.z + 1.0; }
		if (i1.z < i0.z) { a.z = i0.z; }
		float3 b = one3f * 999.0;
		float3 d = v1 - v0;
		if (a.x != 999.0) { b.x = (a.x - v0.x) / d.x; }
		if (a.y != 999.0) { b.y = (a.y - v0.y) / d.y; }
		if (a.z != 999.0) { b.z = (a.z - v0.z) / d.z; }
		int c;
		if (b.x < b.y && b.x < b.z)
		{
			c = i1.x > i0.x ? 4 : 5;
			v0.x = a.x;
			v0.yz += d.yz * b.x;
		}
		else if (b.y < b.z)
		{
			c = i1.y > i0.y ? 0 : 1;
			v0.y = a.y;
			v0.xz += d.xz * b.y;
		}
		else
		{
			c = i1.z > i0.z ? 2 : 3;
			v0.z = a.z;
			v0.xy += d.xy * b.z;
		}
		float3 v00 = (float3){ (int)v0.x, (int)v0.y, (int)v0.z };
		i0 = int3f(v00);
		if (c == 5) { i0.x--; v00.x++; }
		if (c == 1) { i0.y--; v00.y++; }
		if (c == 3) { i0.z--; v00.z++; }
		BlockType tile = LevelGetTile(level, i0.x, i0.y, i0.z);
		Block block = Blocks.Table[tile];
		if (tile != BlockTypeNone && BlockGetLiquidType(block) == LiquidTypeNone)
		{
			MovingObjectPosition pos = BlockClip(block, i0.x, i0.y, i0.z, v0, v1);
			if (!pos.Null) { return pos; }
		}
	}
	return (MovingObjectPosition){ .Null = true };
}

void LevelPlaySound(Level level, const char * sound, Entity entity, float volume, float pitch)
{
	
}

void LevelPlaySoundAt(Level level, const char * sound, float3 position, float volume, float pitch)
{
	
}

bool LevelMaybeGrowTree(Level level, int x, int y, int z)
{
	int r = (int)RandomGeneratorIntegerRange(level->Random, 0, 2) + 4;
	bool grow = true;
	int i, j, k;
	for (j = y; j <= y + 1 + r; ++j)
	{
		int offset = 1;
		if (j == y) { offset = 0; }
		if (j >= y + 1 + r - 2) { offset = 2; }

		for (i = x - offset; i <= x + offset && grow; ++i)
		{
			for (k = z - offset; k <= z + offset && grow; ++k)
			{
				if (i >= 0 && j >= 0 && k >= 0 && i < level->Width && j < level->Depth && k < level->Height)
				{
					if ((level->Blocks[(j * level->Height + k) * level->Width + i] & 255) != 0) { grow = false; }
				}
				else { grow = false; }
			}
		}
	}

	if (!grow) { return false; }
	else if ((level->Blocks[((y - 1) * level->Height + z) * level->Width + x] & 255) == BlockTypeGrass && y < level->Depth - r - 1)
	{
		LevelSetTile(level, x, y - 1, z, BlockTypeDirt);
		int l;
		for (l = y - 3 + r; l <= y + r; ++l)
		{
			i = l - (y + r);
			k = 1 - i / 2;
			for (int m = x - k; m <= x + k; ++m)
			{
				int var12 = m - x;
				for (j = z - k; j <= z + k; ++j)
				{
					int var11 = j - z;
					if (abs(var12) != k || abs(var11) != k || (RandomGeneratorIntegerRange(level->Random, 0, 1) != 0 && i != 0)) { LevelSetTile(level, m, l, j, BlockTypeLeaves); }
				}
			}
		}
		for (l = 0; l < r; ++l) { LevelSetTile(level, x, y + l, z, BlockTypeLog); }

		return true;
	}
	else { return false; }
}

Entity LevelGetPlayer(Level level)
{
	return level->Player;
}

void LevelAddEntity(Level level, Entity entity)
{
	BlockMapInsert(level->BlockMap, entity);
	EntitySetLevel(entity, level);
}

void LevelRemoveEntity(Level level, Entity entity)
{
	BlockMapRemove(level->BlockMap, entity);
}

void LevelExplode(Level level, Entity entity, float3 pos, float radius)
{
	int3 v0 = int3f(pos - radius - 1.0);
	int3 v1 = int3f(pos + radius + 1.0);
	
	for (int i = v0.x; i < v1.x; i++)
	{
		for (int j = v1.y - 1; j >= v0.y; j--)
		{
			for (int k = v0.z; k < v1.z; k++)
			{
				float3 v = (float3){ i, j, k } + 0.5 - pos;
				BlockType tile = LevelGetTile(level, i, j, k);
				if (i >= 0 && j >= 0 && k >= 0 && i < level->Width && j < level->Depth && k < level->Height && length3f(v) < radius && tile > 0 && BlockCanExplode(Blocks.Table[tile]))
				{
					BlockDropItems(Blocks.Table[tile], level, i, j, k, 0.3);
					LevelSetTile(level, i, j, k, BlockTypeNone);
					BlockExplode(Blocks.Table[tile], level, i, j, k);
				}
			}
		}
	}
	
	list(Entity) entities = BlockMapGetEntities(level->BlockMap, entity, float3i(v0), float3i(v1));
	for (int i = 0; i < ListCount(entities); i++)
	{
		float d = EntityDistanceToPoint(entities[i], pos) / radius;
		if (d <= 1.0) { EntityHurt(entities[i], entity, (1.0 - d) * 15.0 + 1.0); }
	}
}

Entity LevelFindPlayer(Level level)
{
	for (int i = 0; i < ListCount(level->BlockMap->All); i++)
	{
		Entity entity = level->BlockMap->All[i];
		if (((MobData)entity->TypeData)->Type == MobTypePlayer) { return entity; }
	}
	return NULL;
}

void LevelRemoveAllNonCreativeModeEntities(Level level)
{
	BlockMapRemoveAllNonCreativeModeEntities(level->BlockMap);
}

void LevelDestroy(Level level)
{
	ListDestroy(level->Renderers);
	RandomGeneratorDestroy(level->Random);
	ListDestroy(level->TickList);
	if (level->LightBlockers != NULL) { MemoryFree(level->LightBlockers); }
	if (level->BlockMap != NULL) { BlockMapDestroy(level->BlockMap); }
	if (level->Blocks != NULL) { MemoryFree(level->Blocks); }
	MemoryFree(level);
}
