#include "Chunk.h"
#include "ShapeRenderer.h"
#include "../Utilities/Log.h"
#include "../Utilities/OpenGL.h"

int ChunkUpdates = 0;

Chunk ChunkCreate(Level level, int3 pos, int chunkSize, int baseListID)
{
	Chunk chunk = MemoryAllocate(sizeof(struct Chunk));
	*chunk = (struct Chunk)
	{
		.Visible = false,
		.Level = level,
		.Position = pos,
		.Width = 16,
		.Height = 16,
		.Depth = 16,
		.BaseListID = baseListID,
	};
	ChunkSetAllDirty(chunk);
	return chunk;
}

void ChunkUpdate(Chunk chunk)
{
	ChunkUpdates++;
	int3 v0 = chunk->Position;
	int3 v1 = chunk->Position + (int3){ chunk->Width, chunk->Height, chunk->Depth };
	
	for (int i = 0; i < 2; i++) { chunk->Dirty[i] = true; }
	for (int i = 0; i < 2; i++)
	{
		bool b0 = false;
		bool b1 = false;
		glNewList(chunk->BaseListID + i, GL_COMPILE);
		ShapeRendererBegin();
		for (int x = v0.x; x < v1.x; x++)
		{
			for (int y = v0.y; y < v1.y; y++)
			{
				for (int z = v0.z; z < v1.z; z++)
				{
					BlockType tile = LevelGetTile(chunk->Level, x, y, z);
					if (tile != BlockTypeNone)
					{
						Block block = Blocks.Table[tile];
						if (BlockGetRenderPass(block) != i) { b0 = true; }
						else { b1 |= BlockRender(block, chunk->Level, x, y, z); }
					}
				}
			}
		}
		ShapeRendererEnd();
		glEndList();
		if (b1) { chunk->Dirty[i] = false; }
		if (!b0) { break; }
	}
}

float ChunkDistanceSquared(Chunk chunk, Player player)
{
	return sqdistance3f(player->Position, float3i(chunk->Position));
}

void ChunkSetAllDirty(Chunk chunk)
{
	for (int i = 0; i < 2; i++) { chunk->Dirty[i] = true; }
}

void ChunkDispose(Chunk chunk)
{
	ChunkSetAllDirty(chunk);
	ChunkDestroy(chunk);
}

int ChunkAppendLists(Chunk chunk, int dataCache[], int count, int pass)
{
	if (!chunk->Visible) { return count; }
	else
	{
		if (!chunk->Dirty[pass]) { dataCache[count++] = chunk->BaseListID + pass; }
		return count;
	}
}

void ChunkClip(Chunk chunk, Frustum frustum)
{
	float3 p = float3i(chunk->Position);
	chunk->Visible = FrustumContainsBox(frustum, p, p + (float3){ chunk->Width, chunk->Height, chunk->Depth });
}

void ChunkDestroy(Chunk chunk)
{
	MemoryFree(chunk);
}

int ChunkDistanceComparator(const void * a, const void * b)
{
	float distA = sqdistance3f(float3i((*(Chunk *)a)->Position), (*(Chunk *)a)->Level->Player->Position);
	float distB = sqdistance3f(float3i((*(Chunk *)b)->Position), (*(Chunk *)a)->Level->Player->Position);
	return distA == distB ? 0 : (distA > distB ? 1 : -1);
}

int ChunkVisibleDistanceComparator(const void * a, const void * b)
{
	if ((*(Chunk *)a)->Visible || !(*(Chunk *)b)->Visible)
	{
		if ((*(Chunk *)b)->Visible)
		{
			float distA = sqdistance3f(float3i((*(Chunk *)a)->Position), (*(Chunk *)a)->Level->Player->Position);
			float distB = sqdistance3f(float3i((*(Chunk *)b)->Position), (*(Chunk *)a)->Level->Player->Position);
			return distA == distB ? 0 : (distA > distB ? 1 : -1);
		}
		else { return -1; }
	}
	else { return 1; }
}
