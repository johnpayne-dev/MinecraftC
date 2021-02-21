#pragma once
#include "TileSound.h"
#include "../../Utilities/LinearMath.h"
#include "../../Utilities/Random.h"
#include "../../Physics/AABB.h"
#include "../../Particle/ParticleManager.h"

typedef enum BlockType
{
	BlockTypeNone,
	BlockTypeStone,
	BlockTypeGrass,
	BlockTypeDirt,
	BlockTypeCobbleStone,
	BlockTypeWood,
	BlockTypeSapling,
	BlockTypeBedrock,
	BlockTypeWater,
	BlockTypeStillWater,
	BlockTypeLava,
	BlockTypeStillLava,
	BlockTypeSand,
	BlockTypeGravel,
	BlockTypeGoldOre,
	BlockTypeIronOre,
	BlockTypeCoalOre,
	BlockTypeLog,
	BlockTypeLeaves,
	BlockTypeSponge,
	BlockTypeGlass,
	BlockTypeRedWool,
	BlockTypeOrangeWool,
	BlockTypeYellowWool,
	BlockTypeLimeWool,
	BlockTypeGreenWool,
	BlockTypeAquaGreenWool,
	BlockTypeCyanWool,
	BlockTypeBlueWool,
	BlockTypePurpleWool,
	BlockTypeIndigoWool,
	BlockTypeVioletWool,
	BlockTypeMagentaWool,
	BlockTypePinkWool,
	BlockTypeBlackWool,
	BlockTypeGrayWool,
	BlockTypeWhiteWool,
	BlockTypeDandelion,
	BlockTypeRose,
	BlockTypeBrownMushroom,
	BlockTypeRedMushroom,
	BlockTypeGold,
	BlockTypeIron,
	BlockTypeDoubleSlab,
	BlockTypeSlab,
	BlockTypeBrick,
	BlockTypeTNT,
	BlockTypeBookshelf,
	BlockTypeMossyCobbleStone,
	BlockTypeObsidian,
	BlockTypeCount,
} BlockType;

typedef enum LiquidType
{
	LiquidTypeNone,
	LiquidTypeWater,
	LiquidTypeLava,
} LiquidType;

typedef struct Block
{
	int TextureID;
	TileSound Sound;
	int Hardness;
	bool Explodable;
	float3 XYZ0;
	float3 XYZ1;
	float ParticleGravity;
	BlockType Type;
	void * TypeData;
} * Block;

Block BlockCreate(BlockType type, int textureID);
bool BlockIsCube(Block block);
Block BlockSetData(Block block, TileSound sound, float var, float particleGravity, float hardness);
void BlockSetPhysics(Block block, bool physics);
void BlockSetBounds(Block block, float3 v0, float3 v1);
void BlockSetTickDelay(Block block, int tickDelay);
void BlockRenderFullBrightness(Block block);
float BlockGetBrightness(Block block, struct Level * level, int x, int y, int z);
bool BlockCanRenderSide(Block block, struct Level * level, int x, int y, int z, int side);
int BlockGetTextureID(Block block, int side);
void BlockRenderInside(Block block, int x, int y, int z, int side);
void BlockRenderSideWithTexture(Block block, int x, int y, int z, int side, int tex);
void BlockRenderSide(Block block, int x, int y, int z, int side);
AABB BlockGetSelectionAABB(Block block, int x, int y, int z);
AABB BlockGetCollisionAABB(Block block, int x, int y, int z);
bool BlockIsOpaque(Block block);
bool BlockIsSolid(Block block);
void BlockUpdate(Block block, struct Level * level, int x, int y, int z, RandomGenerator random);
void BlockSpawnBreakParticles(Block block, struct Level * level, int x, int y, int z, ParticleManager particles);
void BlockSpawnBlockParticles(Block block, struct Level * level, int x, int y, int z, int side, ParticleManager particles);
LiquidType BlockGetLiquidType(Block block);
void BlockOnNeighborChanged(Block block, struct Level * level, int x, int y, int z, BlockType tile);
void BlockOnPlaced(Block block, struct Level * level, int x, int y, int z);
int BlockGetTickDelay(Block block);
void BlockOnAdded(Block block, struct Level * level, int x, int y, int z);
void BlockOnRemoved(Block block, struct Level * level, int x, int y, int z);
int BlockGetDropCount(Block block);
BlockType BlockGetDrop(Block block);
int BlockGetHardness(Block block);
void BlockOnBreak(Block block, struct Level * level, int x, int y, int z);
void BlockDropItems(Block block, struct Level * level, int x, int y, int z, float probability);
void BlockRenderPreview(Block block);
bool BlockCanExplode(Block block);
MovingObjectPosition BlockClip(Block block, int x, int y, int z, float3 var1, float3 var2);
void BlockExplode(Block block, struct Level * level, int x, int y, int z);
bool BlockRender(Block block, struct Level * level, int x, int y, int z);
int BlockGetRenderPass(Block block);
void BlockDestroy(Block block);

extern struct Blocks
{
	Block Table[256];
	bool Physics[256];
	bool Opaque[256];
	bool Cube[256];
	bool Liquid[256];
	int TickDelay[256];
} Blocks;

void BlocksInitialize(void);
void BlocksDeinitialize(void);
