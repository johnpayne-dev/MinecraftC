#include <string.h>
#include "Level.h"
#include "../Render/LevelRenderer.h"
#include "../Utilities/Log.h"
#include "../Utilities/SinTable.h"
#include "../Particle/PrimedTNT.h"
#include "../Minecraft.h"

void LevelCreate(Level * level, ProgressBarDisplay * progressBar, int size) {
	*level = (Level) {
		.tickList = ListCreate(sizeof(NextTickListEntry)),
		.entities = ListCreate(sizeof(Entity *)),
		.unprocessed = 0,
		.tickCount = 0,
		.skyColor = 0x99CCFFFF,
		.fogColor = 0xffffffff,
		.cloudColor = 0xffffffff,
	};
	RandomGeneratorCreate(&level->random, time(NULL));
	level->randomValue = (int)RandomGeneratorInteger(&level->random);
	LevelGeneratorCreate(&level->generator, progressBar);
	LevelGeneratorGenerate(&level->generator, 128 << size, 128 << size, level);
}

bool LevelLoad(Level * level, char * filePath) {
	SDL_RWops * file = SDL_RWFromFile(filePath, "rb");
	if (file == NULL) {
		return false;
	}
	char nameChar;
	do { SDL_RWread(file, &nameChar, 1, 1); } while (nameChar != '\0');
	int x, y, z, w, d, h;
	float r;
	SDL_RWread(file, &x, sizeof(x), 1);
	SDL_RWread(file, &y, sizeof(y), 1);
	SDL_RWread(file, &z, sizeof(z), 1);
	SDL_RWread(file, &r, sizeof(r), 1);
	SDL_RWread(file, &w, sizeof(w), 1);
	SDL_RWread(file, &d, sizeof(d), 1);
	SDL_RWread(file, &h, sizeof(h), 1);
	uint8_t * blocks = malloc(w * d * h);
	SDL_RWread(file, blocks, w * d * h, 1);
	LevelSetData(level, w, d, h, blocks);
	free(blocks);
	level->xSpawn = x;
	level->ySpawn = y;
	level->zSpawn = z;
	level->spawnRotation = r;
	level->tickList = ListClear(level->tickList);
	level->entities = ListClear(level->entities);
	level->entities = ListPush(level->entities, &level->player);
	level->unprocessed = 0;
	level->tickCount = 0;
	EntityResetPosition(level->player);
	return true;
}

bool LevelSave(Level * level, char * filePath, char * name) {
	SDL_RWops * file = SDL_RWFromFile(filePath, "wb");
	if (file == NULL) {
		return false;
	}
	SDL_RWwrite(file, name, strlen(name) + 1, 1);
	SDL_RWwrite(file, &level->xSpawn, sizeof(level->xSpawn), 1);
	SDL_RWwrite(file, &level->ySpawn, sizeof(level->ySpawn), 1);
	SDL_RWwrite(file, &level->zSpawn, sizeof(level->zSpawn), 1);
	SDL_RWwrite(file, &level->spawnRotation, sizeof(level->spawnRotation), 1);
	SDL_RWwrite(file, &level->width, sizeof(level->width), 1);
	SDL_RWwrite(file, &level->depth, sizeof(level->depth), 1);
	SDL_RWwrite(file, &level->height, sizeof(level->height), 1);
	SDL_RWwrite(file, level->blocks, level->width * level->depth * level->height, 1);
	SDL_RWclose(file);
	return true;
}

void LevelRegenerate(Level * level, int size) {
	LevelDestroy(level);
	LevelCreate(level, level->generator.progressBar, size);
}

void LevelSetData(Level * level, int w, int d, int h, uint8_t * blocks) {
	level->width = w;
	level->depth = d;
	level->height = h;
	level->blocks = malloc(w * d * h);
	memcpy(level->blocks, blocks, w * d * h);
	level->lightBlockers = malloc(w * h * sizeof(int));
	for (int i = 0; i < w * h; i++) {
		level->lightBlockers[i] = level->depth - 1;
	}
	LevelCalculateLightDepths(level, 0, 0, w, h);
	if (level->renderer != NULL) {
		LevelRendererRefresh(level->renderer);
	}
	level->tickList = ListClear(level->tickList);
	LevelFindSpawn(level);
}

void LevelFindSpawn(Level * level) {
	int i = 0;
	int x, y, z;
	do {
		i++;
		x = RandomIntegerRange(level->width / 4, level->width / 2);
		z = RandomIntegerRange(level->height / 4, level->height / 2);
		y = LevelGetHighestTile(level, x, z) + 1;
		if (i == 10000) {
			level->xSpawn = x;
			level->ySpawn = -100;
			level->zSpawn = z;
			return;
		}
	} while (y <= LevelGetWaterLevel(level));
	level->xSpawn = x;
	level->ySpawn = y;
	level->zSpawn = z;
	return;
}

void LevelCalculateLightDepths(Level * level, int x0, int y0, int x1, int y1) {
	for (int i = x0; i < x0 + x1; i++) {
		for (int j = y0; j < y0 + y1; j++) {
			int blocker = level->lightBlockers[i + j * level->width];
			int k = level->depth - 1;
			while (!LevelIsLightBlocker(level, i, k, j) && k > 0) { k--; }
			level->lightBlockers[i + j * level->width] = k;
			if (blocker != k) {
				int min = blocker < k ? blocker : k;
				int max = blocker > k ? blocker : k;
				if (level->renderer != NULL) { LevelRendererQueueChunks(level->renderer, i - 1, min - 1, j - 1, i + 1, max + 1, j + 1); }
			}
		}
	}
}

void LevelSetRenderer(Level * level, LevelRenderer * listener) {
	level->renderer = listener;
}

bool LevelIsLightBlocker(Level * level, int x, int y, int z) {
	Block * block = &Blocks.table[LevelGetTile(level, x, y, z)];
	return block->type == BlockTypeNone ? false : BlockIsOpaque(block);
}

List(AABB) LevelGetCubes(Level * level, AABB box) {
	List(AABB) list = ListCreate(sizeof(AABB));
	int x0 = box.x0, y0 = box.y0, z0 = box.z0;
	int x1 = box.x1 + 1, y1 = box.y1 + 1, z1 = box.z1 + 1;
	if (box.x0 < 0.0) { x0--; }
	if (box.y0 < 0.0) { y0--; }
	if (box.z0 < 0.0) { z0--; }
	for (int i = x0; i < x1; i++) {
		for (int j = y0; j < y1; j++) {
			for (int k = z0; k < z1; k++) {
				AABB aabb = { 0 };
				if (i >= 0 && j >= 0 && k >= 0 && i < level->width && j < level->depth && k < level->height) {
					Block * block = &Blocks.table[LevelGetTile(level, i, j, k)];
					if (block->type != BlockTypeNone) {
						aabb = BlockGetCollisionAABB(block, i, j, k);
						if (!aabb.null && AABBIntersectsInner(box, aabb)) { list = ListPush(list, &aabb); }
					}
				} else if (i < 0 || j < 0 || k < 0 || i >= level->width || k >= level->height) {
					AABB aabb = BlockGetCollisionAABB(&Blocks.table[BlockTypeBedrock], i, j, k);
					if (!aabb.null && AABBIntersectsInner(box, aabb)) { list = ListPush(list, &aabb); }
				}
			}
		}
	}
	return list;
}

void LevelSwap(Level * level, int x0, int y0, int z0, int x1, int y1, int z1) {
	BlockType t1 = LevelGetTile(level, x0, y0, z0);
	BlockType t2 = LevelGetTile(level, x1, y1, z1);
	LevelSetTileNoNeighborChange(level, x0, y0, z0, t2);
	LevelSetTileNoNeighborChange(level, x1, y1, z1, t1);
	LevelUpdateNeighborsAt(level, x0, y0, z0, t2);
	LevelUpdateNeighborsAt(level, x1, y1, z1, t1);
}

bool LevelSetTile(Level * level, int x, int y, int z, BlockType tile) {
	if (LevelSetTileNoNeighborChange(level, x, y, z, tile)) {
		LevelUpdateNeighborsAt(level, x, y, z, tile);
		return true;
	}
	else { return false; }
}

bool LevelSetTileNoNeighborChange(Level * level, int x, int y, int z, BlockType tile) {
	if (x < 0 || y < 0 || z < 0 || x >= level->width || y >= level->depth || z >= level->height) { return false; }
	int i = (y * level->height + z) * level->width + x;
	if (tile == level->blocks[i]) { return false; }
	
	if (tile == BlockTypeNone && (x == 0 || z == 0 || x == level->width - 1 || z == level->height - 1) && y >= LevelGetGroundLevel(level) && y < LevelGetWaterLevel(level)) { tile = BlockTypeWater; }
	BlockType prev = level->blocks[i];
	level->blocks[i] = tile;
	if (prev != BlockTypeNone) { BlockOnRemoved(&Blocks.table[prev], level, x, y, z); }
	if (tile != BlockTypeNone) { BlockOnAdded(&Blocks.table[tile], level, x, y, z); }
	LevelCalculateLightDepths(level, x, z, 1, 1);
	if (level->renderer != NULL) { LevelRendererQueueChunks(level->renderer, x - 1, y - 1, z - 1, x + 1, y + 1, z + 1); }
	return true;
}

static void UpdateTile(Level * level, int x, int y, int z, BlockType tile) {
	if (x < 0 || y < 0 || z < 0 || x >= level->width || y >= level->depth || z >= level->height) { return; }
	Block * block = &Blocks.table[level->blocks[(y * level->height + z) * level->width + x]];
	if (block->type != BlockTypeNone) { BlockOnNeighborChanged(block, level, x, y, z, tile); }
}

void LevelUpdateNeighborsAt(Level * level, int x, int y, int z, BlockType tile) {
	UpdateTile(level, x - 1, y, z, tile);
	UpdateTile(level, x + 1, y, z, tile);
	UpdateTile(level, x, y - 1, z, tile);
	UpdateTile(level, x, y + 1, z, tile);
	UpdateTile(level, x, y, z - 1, tile);
	UpdateTile(level, x, y, z + 1, tile);
}

bool LevelSetTileNoUpdate(Level * level, int x, int y, int z, BlockType tile) {
	if (x < 0 || y < 0 || z < 0 || x >= level->width || y >= level->depth || z >= level->height) { return false; }
	int i = (y * level->height + z) * level->width + x;
	if (tile == level->blocks[i]) { return false; }
	level->blocks[i] = tile;
	return true;
}

bool LevelIsLit(Level * level, int x, int y, int z) {
	return !(x < 0 || y < 0 || z < 0 || x >= level->width || y >= level->depth || z >= level->height) ? y >= level->lightBlockers[x + z * level->width] : true;
}

BlockType LevelGetTile(Level * level, int x, int y, int z) {
	return !(x < 0 || y < 0 || z < 0 || x >= level->width || y >= level->depth || z >= level->height) ? level->blocks[(y * level->height + z) * level->width + x] & 0xFF : 0;
}

bool LevelIsSolidTile(Level * level, int x, int y, int z) {
	Block * block = &Blocks.table[LevelGetTile(level, x, y, z)];
	return block->type == BlockTypeNone ? false : BlockIsSolid(block);
}

void LevelTickEntities(Level * level) {
	for (int i = 0; i < ListLength(level->entities); i++) {
		EntityTick(level->entities[i]);
		if (level->entities[i]->removed) { level->entities = ListRemove(level->entities, i--); }
	}
}

void LevelTick(Level * level) {
	level->tickCount++;
	
	int a = 1;
	int b = 1;
	for (b = 1; 1 << a < level->width; a++);
	while (1 << b < level->height) { b++; }
	
	if (level->tickCount % 5 == 0) {
		for (int i = 0; i < ListLength(level->tickList); i++) {
			NextTickListEntry nextTick = level->tickList[0];
			level->tickList = ListRemove(level->tickList, 0);
			if (nextTick.Ticks > 0) {
				nextTick.Ticks--;
				level->tickList = ListPush(level->tickList, &nextTick);
			} else {
				BlockType tile = level->blocks[(nextTick.y * level->height + nextTick.z) * level->width + nextTick.x];
				if (LevelIsInBounds(level, nextTick.x, nextTick.y, nextTick.z) && tile == nextTick.Tile && tile != BlockTypeNone) {
					BlockUpdate(&Blocks.table[tile], level, nextTick.x, nextTick.y, nextTick.z, &level->random);
				}
			}
		}
	}
	
	level->unprocessed += level->width * level->height * level->depth;
	int c = level->unprocessed / 200;
	level->unprocessed -= c * 200;
	
	for (int i = 0; i < c; i++) {
		level->randomValue = level->randomValue * 3 + 1013904223;
		int y = level->randomValue >> 2;
		int x = y & (level->width - 1);
		int z = y >> a & (level->height - 1);
		y = y >> (a + b) & (level->depth - 1);
		BlockType tile = level->blocks[(y * level->height + z) * level->width + x];
		if (Blocks.physics[tile]) { BlockUpdate(&Blocks.table[tile], level, x, y, z, &level->random); }
	}
}

bool LevelIsInBounds(Level * level, int x, int y, int z) {
	return !(x < 0 || y < 0 || z < 0 || x >= level->width || y >= level->depth || z >= level->height);
}

float LevelGetGroundLevel(Level * level) {
	return LevelGetWaterLevel(level) - 2.0;
}

float LevelGetWaterLevel(Level * level) {
	return level->waterLevel;
}

bool LevelContainsAnyLiquid(Level * level, AABB box) {
	int x0 = box.x0, y0 = box.y0, z0 = box.z0;
	int x1 = box.x1 + 1, y1 = box.y1 + 1, z1 = box.z1 + 1;
	if (box.x0 < 0.0) { x0--; }
	if (box.y0 < 0.0) { y0--; }
	if (box.z0 < 0.0) { z0--; }
	if (x0 < 0) { x0 = 0; }
	if (y0 < 0) { y0 = 0; }
	if (z0 < 0) { z0 = 0; }
	if (x1 > level->width) { x1 = level->width; }
	if (y1 > level->depth) { y1 = level->depth; }
	if (z1 > level->height) { z1 = level->height; }
	for (int i = x0; i < x1; i++) {
		for (int j = y0; j < y1; j++) {
			for (int k = z0; k < z1; k++) {
				Block * block = &Blocks.table[LevelGetTile(level, i, j, k)];
				if (block->type != BlockTypeNone && BlockGetLiquidType(block) != LiquidTypeNone) { return true; }
			}
		}
	}
	return false;
}

bool LevelContainsLiquid(Level * level, AABB box, LiquidType liquidID) {
	int x0 = box.x0, y0 = box.y0, z0 = box.z0;
	int x1 = box.x1 + 1, y1 = box.y1 + 1, z1 = box.z1 + 1;
	if (box.x0 < 0.0) { x0--; }
	if (box.y0 < 0.0) { y0--; }
	if (box.z0 < 0.0) { z0--; }
	if (x0 < 0) { x0 = 0; }
	if (y0 < 0) { y0 = 0; }
	if (z0 < 0) { z0 = 0; }
	if (x1 > level->width) { x1 = level->width; }
	if (y1 > level->depth) { y1 = level->depth; }
	if (z1 > level->height) { z1 = level->height; }
	for (int i = x0; i < x1; i++) {
		for (int j = y0; j < y1; j++) {
			for (int k = z0; k < z1; k++) {
				Block * block = &Blocks.table[LevelGetTile(level, i, j, k)];
				if (block->type != BlockTypeNone && BlockGetLiquidType(block) == liquidID) { return true; }
			}
		}
	}
	return false;
}

void LevelAddToNextTick(Level * level, int x, int y, int z, BlockType tile) {
	NextTickListEntry tick = { .x = x, .y = y, .z = z, .Tile = tile };
	if (tile != BlockTypeNone) { tick.Ticks = BlockGetTickDelay(&Blocks.table[tile]); }
	level->tickList = ListPush(level->tickList, &tick);
}

static bool IsSolid(Level * level, float x, float y, float z) {
	BlockType tile = LevelGetTile(level, x, y, z);
	return tile != BlockTypeNone && BlockIsSolid(&Blocks.table[tile]);
}

bool LevelIsSolidSearch(Level * level, float x, float y, float z, float r) {
	if (IsSolid(level, x - r, y - r, z - r)) { return true; }
	if (IsSolid(level, x + r, y - r, z - r)) { return true; }
	if (IsSolid(level, x - r, y + r, z - r)) { return true; }
	if (IsSolid(level, x + r, y + r, z - r)) { return true; }
	if (IsSolid(level, x - r, y - r, z + r)) { return true; }
	if (IsSolid(level, x + r, y - r, z + r)) { return true; }
	if (IsSolid(level, x - r, y + r, z + r)) { return true; }
	if (IsSolid(level, x + r, y + r, z + r)) { return true; }
	return false;
}

int LevelGetHighestTile(Level * level, int x, int z) {
	int y;
	for (y = level->depth; (LevelGetTile(level, x, y - 1, z) == 0 || BlockGetLiquidType(&Blocks.table[LevelGetTile(level, x, y - 1, z)]) != LiquidTypeNone) && y > 0; y--);
	return y;
}

void LevelSetSpawnPosition(Level * level, int x, int y, int z, float rotation) {
	level->xSpawn = x;
	level->ySpawn = y;
	level->zSpawn = z;
	level->spawnRotation = rotation;
}

float LevelGetBrightness(Level * level, int x, int y, int z) {
	return LevelIsLit(level, x, y, z) ? 1.0 : 0.6;
}

bool LevelIsWater(Level * level, int x, int y, int z) {
	BlockType tile = LevelGetTile(level, x, y, z);
	return tile != BlockTypeNone && BlockGetLiquidType(&Blocks.table[tile]) == LiquidTypeWater;
}

MovingObjectPosition LevelClip(Level * level, Vector3D v0, Vector3D v1) {
	if (v0.x == NAN || v0.y == NAN || v0.z == NAN) { return (MovingObjectPosition){ .null = true }; }
	if (v1.x == NAN || v1.y == NAN || v1.z == NAN) { return (MovingObjectPosition){ .null = true }; }
	int i0x = v0.x, i0y = v0.y, i0z = v0.z;
	int i1x = v1.x, i1y = v1.y, i1z = v1.z;
	
	int i = 20;
	while (i-- >= 0) {
		if (v0.x == NAN || v0.y == NAN || v0.z == NAN) { return (MovingObjectPosition){ .null = true }; }
		if (i0x == i1x && i0y == i1y && i0z == i1z) { return (MovingObjectPosition){ .null = true }; }
		Vector3D a = { 999.0, 999.0, 999.0 };
		if (i1x > i0x) { a.x = i0x + 1.0; }
		if (i1x < i0x) { a.x = i0x; }
		if (i1y > i0y) { a.y = i0y + 1.0; }
		if (i1y < i0y) { a.y = i0y; }
		if (i1z > i0z) { a.z = i0z + 1.0; }
		if (i1z < i0z) { a.z = i0z; }
		Vector3D b = { 999.0, 999.0, 999.0 };
		Vector3D d = Vector3DSubtract(v1, v0);
		if (a.x != 999.0) { b.x = (a.x - v0.x) / d.x; }
		if (a.y != 999.0) { b.y = (a.y - v0.y) / d.y; }
		if (a.z != 999.0) { b.z = (a.z - v0.z) / d.z; }
		int c;
		if (b.x < b.y && b.x < b.z) {
			c = i1x > i0x ? 4 : 5;
			v0.x = a.x;
			v0.y += d.y * b.x;
			v0.z += d.z * b.x;
		} else if (b.y < b.z) {
			c = i1y > i0y ? 0 : 1;
			v0.y = a.y;
			v0.x += d.x * b.y;
			v0.z += d.z * b.y;
		} else {
			c = i1z > i0z ? 2 : 3;
			v0.z = a.z;
			v0.x += d.x * b.z;
			v0.y += d.y * b.z;
		}
		Vector3D v00 = { (int)v0.x, (int)v0.y, (int)v0.z };
		i0x = v00.x;
		i0y = v00.y;
		i0z = v00.z;
		if (c == 5) { i0x--; v00.x++; }
		if (c == 1) { i0y--; v00.y++; }
		if (c == 3) { i0z--; v00.z++; }
		BlockType tile = LevelGetTile(level, i0x, i0y, i0z);
		Block * block = &Blocks.table[tile];
		if (tile != BlockTypeNone && BlockGetLiquidType(block) == LiquidTypeNone) {
			MovingObjectPosition pos = BlockClip(block, i0x, i0y, i0z, v0, v1);
			if (!pos.null) { return pos; }
		}
	}
	return (MovingObjectPosition){ .null = true };
}

void LevelPlaySound(Level * level, char * sound, Entity * entity, float volume, float pitch) {
	GameSettings * settings = level->renderer->textures->settings;
	if (!settings->sound) { return; }
	if (Vector3DSqDistance((Vector3D){ entity->x, entity->y, entity->z }, (Vector3D){ level->player->x, level->player->y, level->player->z }) < 1024.0) {
		SoundManagerPlaySound(&settings->minecraft->sound, sound, volume, pitch);
	}
}

void LevelPlaySoundAt(Level * level, char * sound, float x, float y, float z, float volume, float pitch) {
	GameSettings * settings = level->renderer->textures->settings;
	if (!settings->sound) { return; }
	SoundManagerPlaySound(&settings->minecraft->sound, sound, volume, pitch);
}

bool LevelMaybeGrowTree(Level * level, int x, int y, int z) {
	int r = (int)RandomGeneratorIntegerRange(&level->random, 0, 2) + 4;
	bool grow = true;
	int i, j, k;
	for (j = y; j <= y + 1 + r; ++j) {
		int offset = 1;
		if (j == y) { offset = 0; }
		if (j >= y + 1 + r - 2) { offset = 2; }

		for (i = x - offset; i <= x + offset && grow; ++i) {
			for (k = z - offset; k <= z + offset && grow; ++k) {
				if (i >= 0 && j >= 0 && k >= 0 && i < level->width && j < level->depth && k < level->height) {
					if ((level->blocks[(j * level->height + k) * level->width + i] & 255) != 0) { grow = false; }
				} else { grow = false; }
			}
		}
	}

	if (!grow) { return false; }
	else if ((level->blocks[((y - 1) * level->height + z) * level->width + x] & 255) == BlockTypeGrass && y < level->depth - r - 1) {
		LevelSetTile(level, x, y - 1, z, BlockTypeDirt);
		int l;
		for (l = y - 3 + r; l <= y + r; ++l) {
			i = l - (y + r);
			k = 1 - i / 2;
			for (int m = x - k; m <= x + k; ++m) {
				int var12 = m - x;
				for (j = z - k; j <= z + k; ++j) {
					int var11 = j - z;
					if (abs(var12) != k || abs(var11) != k || (RandomGeneratorIntegerRange(&level->random, 0, 1) != 0 && i != 0)) { LevelSetTile(level, m, l, j, BlockTypeLeaves); }
				}
			}
		}
		for (l = 0; l < r; ++l) { LevelSetTile(level, x, y + l, z, BlockTypeLog); }

		return true;
	} else { return false; }
}

Entity * LevelGetPlayer(Level * level) {
	return level->player;
}

void LevelAddEntity(Level * level, Entity * entity) {
	EntitySetLevel(entity, level);
	level->entities = ListPush(level->entities, &entity);
}

void LevelRenderEntities(Level * level, TextureManager * textures, float dt) {
	for (int i = 0; i < ListLength(level->entities); i++) {
		EntityRender(level->entities[i], textures, dt);
	}
}

void LevelExplode(Level * level, float x, float y, float z, float radius) {
	int x0 = x - radius - 1.0;
	int y0 = y - radius - 1.0;
	int z0 = z - radius - 1.0;
	int x1 = x + radius + 1.0;
	int y1 = y + radius + 1.0;
	int z1 = z + radius + 1.0;
	for (int i = x0; i < x1; i++) {
		for (int j = y1 - 1; j >= y0; j--) {
			for (int k = z0; k < z1; k++) {
				float vx = i + 0.5 - x;
				float vy = j + 0.5 - y;
				float vz = k + 0.5 - z;
				BlockType tile = LevelGetTile(level, i, j, k);
				if (i >= 0 && j >= 0 && k >= 0 && i < level->width && j < level->depth && k < level->height && vx * vx + vy * vy + vz * vz < radius * radius && tile > 0 && BlockCanExplode(&Blocks.table[tile])) {
					LevelSetTile(level, i, j, k, BlockTypeNone);
					BlockExplode(&Blocks.table[tile], level, i, j, k);
				}
			}
		}
	}
}

Entity * LevelFindPlayer(Level * level) {
	return level->player;
}

void LevelDestroy(Level * level) {
	ListFree(level->entities);
	ListFree(level->tickList);
	if (level->lightBlockers != NULL) { free(level->lightBlockers); }
	if (level->blocks != NULL) { free(level->blocks); }
}
