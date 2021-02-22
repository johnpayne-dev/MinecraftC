#include <string.h>
#include "BlockMap.h"

BlockMap BlockMapCreate(int width, int depth, int height)
{
	BlockMap map = MemoryAllocate(sizeof(struct BlockMap));
	*map = (struct BlockMap)
	{
		.Width = width / 16,
		.Depth = depth / 16,
		.Height = height / 16,
	};
	if (map->Width == 0) { map->Width = 1; }
	if (map->Depth == 0) { map->Depth = 1; }
	if (map->Height == 0) { map->Height = 1; }
	map->EntityGrid = MemoryAllocate(map->Width * map->Depth * map->Height * sizeof(list(Entity)));
	for (int i = 0; i < map->Width; i++)
	{
		for (int j = 0; j < map->Depth; j++)
		{
			for (int k = 0; k < map->Height; k++)
			{
				map->EntityGrid[(k * map->Depth + j) * map->Width + i] = ListCreate(sizeof(Entity));
			}
		}
	}
	map->Slot1 = BlockMapSlotCreate(map);
	map->Slot2 = BlockMapSlotCreate(map);
	map->All = ListCreate(sizeof(Entity));
	return map;
}

void BlockMapInsert(BlockMap map, Entity entity)
{
	map->All = ListPush(map->All, &entity);
	BlockMapSlotAdd(BlockMapSlotInitialize(map->Slot1, entity->Position), entity);
	entity->OldPosition = entity->Position;
	entity->BlockMap = map;
}

void BlockMapRemove(BlockMap map, Entity entity)
{
	BlockMapSlotRemove(BlockMapSlotInitialize(map->Slot1, entity->OldPosition), entity);
	map->All = ListRemoveAll(map->All, &entity);
}

void BlockMapMoved(BlockMap map, Entity entity)
{
	BlockMapSlot slot1 = BlockMapSlotInitialize(map->Slot1, entity->OldPosition);
	BlockMapSlot slot2 = BlockMapSlotInitialize(map->Slot2, entity->Position);
	if (memcmp(slot1, slot2, sizeof(struct BlockMapSlot)) != 0)
	{
		BlockMapSlotRemove(slot1, entity);
		BlockMapSlotAdd(slot2, entity);
	}
}

list(Entity) BlockMapGetEntities(BlockMap map, Entity entity, float3 v0, float3 v1)
{
	list(Entity) out = ListCreate(sizeof(Entity));
	BlockMapSlot slot1 = BlockMapSlotInitialize(map->Slot1, v0);
	BlockMapSlot slot2 = BlockMapSlotInitialize(map->Slot2, v0);
	for (int i = slot1->Position.x - 1; i <= slot2->Position.x + 1; i++)
	{
		for (int j = slot1->Position.y - 1; j <= slot2->Position.y + 1; j++)
		{
			for (int k = slot1->Position.z - 1; k <= slot2->Position.z + 1; k++)
			{
				if (i >= 0 && j >= 0 && k >= 0 && i < map->Width && j < map->Depth && k < map->Height)
				{
					list(Entity) list = map->EntityGrid[(k * map->Depth + j) * map->Width + i];
					for (int l = 0; l < ListCount(list); l++)
					{
						if (list[l] != entity && EntityIntersects(list[l], v0, v1)) { out = ListPush(out, &list[l]); }
					}
				}
			}
		}
	}
	return out;
}

list(Entity) BlockMapGetEntitiesAABB(BlockMap map, Entity entity, AABB aabb)
{
	return BlockMapGetEntities(map, entity, aabb.V0, aabb.V1);
}

void BlockMapRemoveAllNonCreativeModeEntities(BlockMap map)
{
	for (int i = 0; i < map->Width; i++)
	{
		for (int j = 0; j < map->Depth; j++)
		{
			for (int k = 0; k < map->Height; k++)
			{
				list(Entity) list = map->EntityGrid[(k * map->Depth + j) * map->Width + i];
				for (int l = 0; l < ListCount(list); l++)
				{
					if (!EntityIsCreativeModeAllowed(list[l])) { list = ListRemove(list, l); l--; }
				}
			}
		}
	}
}

void BlockMapClear(BlockMap map)
{
	for (int i = 0; i < map->Width; i++)
	{
		for (int j = 0; j < map->Depth; j++)
		{
			for (int k = 0; k < map->Height; k++)
			{
				list(Entity) list = map->EntityGrid[(k * map->Depth + j) * map->Width + i];
				if (ListCount(list) > 0)
				{
					ListDestroy(list);
					map->EntityGrid[(k * map->Depth + j) * map->Width + i] = ListCreate(sizeof(Entity));
				}
			}
		}
	}
}

void BlockMapTickAll(BlockMap map)
{
	for (int i = 0; i < ListCount(map->All); i++)
	{
		Entity entity = map->All[i];
		EntityTick(entity);
		if (entity->Removed)
		{
			map->All = ListRemove(map->All, i);
			i--;
			BlockMapSlotRemove(BlockMapSlotInitialize(map->Slot1, entity->OldPosition), entity);
		}
		else
		{
			int3 v0 = int3f(entity->OldPosition / 16.0);
			int3 v1 = int3f(entity->Position / 16.0);
			if (v0.x != v1.x || v0.y != v1.y || v0.z != v1.z) { BlockMapMoved(map, entity); }
		}
	}
}

void BlockMapRender(BlockMap map, float3 v, Frustum frustum, TextureManager textures, float t)
{
	for (int i = 0; i < map->Width; i++)
	{
		float x0 = (i << 4) - 2;
		float x1 = ((i + 1) << 4) + 2;
		for (int j = 0; j < map->Depth; j++)
		{
			float y0 = (j << 4) - 2;
			float y1 = ((j + 1) << 4) + 2;
			for (int k = 0; k < map->Height; k++)
			{
				list(Entity) list = map->EntityGrid[(k * map->Depth + j) * map->Width + i];
				if (ListCount(list) == 0) { continue; }
				float z0 = (k << 4) - 2;
				float z1 = ((k + 1) << 4) + 2;
				float3 v0 = { x0, y0, z0 };
				float3 v1 = { x1, y1, z1 };
				if (FrustumContainsBox(frustum, v0, v1))
				{
					bool b = true;
					int c = 0;
					while (c < 6)
					{
						if (frustum.Planes[c][0] * v0.x + frustum.Planes[c][1] * v0.y + frustum.Planes[c][2] * v0.z + frustum.Planes[c][3] <= 0.0) { b = false; break; }
						if (frustum.Planes[c][0] * v1.x + frustum.Planes[c][1] * v0.y + frustum.Planes[c][2] * v0.z + frustum.Planes[c][3] <= 0.0) { b = false; break; }
						if (frustum.Planes[c][0] * v0.x + frustum.Planes[c][1] * v1.y + frustum.Planes[c][2] * v0.z + frustum.Planes[c][3] <= 0.0) { b = false; break; }
						if (frustum.Planes[c][0] * v1.x + frustum.Planes[c][1] * v1.y + frustum.Planes[c][2] * v0.z + frustum.Planes[c][3] <= 0.0) { b = false; break; }
						if (frustum.Planes[c][0] * v0.x + frustum.Planes[c][1] * v0.y + frustum.Planes[c][2] * v1.z + frustum.Planes[c][3] <= 0.0) { b = false; break; }
						if (frustum.Planes[c][0] * v1.x + frustum.Planes[c][1] * v0.y + frustum.Planes[c][2] * v1.z + frustum.Planes[c][3] <= 0.0) { b = false; break; }
						if (frustum.Planes[c][0] * v0.x + frustum.Planes[c][1] * v1.y + frustum.Planes[c][2] * v1.z + frustum.Planes[c][3] <= 0.0) { b = false; break; }
						if (frustum.Planes[c][0] * v1.x + frustum.Planes[c][1] * v1.y + frustum.Planes[c][2] * v1.z + frustum.Planes[c][3] <= 0.0) { b = false; break; }
						c++;
					}
					for (int l = 0; l < ListCount(list); l++)
					{
						if (EntityShouldRender(list[l], v))
						{
							if (!b && !FrustumContainsBox(frustum, list[l]->AABB.V0, list[l]->AABB.V1)) { continue; }
							EntityRender(list[l], textures, t);
						}
					}
				}
			}
		}
	}
}

void BlockMapDestroy(BlockMap map)
{
	for (int i = 0; i < map->Width * map->Depth * map->Height; i++) { ListDestroy(map->EntityGrid[i]); }
	MemoryFree(map->EntityGrid);
	BlockMapSlotDestroy(map->Slot1);
	BlockMapSlotDestroy(map->Slot2);
	ListDestroy(map->All);
	MemoryFree(map);
}
