#pragma once
#include "TileSound.h"
#include "../../Utilities/Random.h"
#include "../../Physics/AABB.h"
#include "../../Particle/ParticleManager.h"

typedef enum BlockType {
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

typedef enum LiquidType {
	LiquidTypeNone,
	LiquidTypeWater,
	LiquidTypeLava,
} LiquidType;

typedef struct Block {
	int textureID;
	TileSound sound;
	bool explodable;
	float x0, y0, z0;
	float x1, y1, z1;
	float particleGravity;
	BlockType type;
	void * typeData;
} * Block;

Block BlockCreate(BlockType type, int textureID);
bool BlockIsCube(Block block);
Block BlockSetData(Block block, TileSound sound, float particleGravity);
void BlockSetPhysics(Block block, bool physics);
void BlockSetBounds(Block block, float x0, float y0, float z0, float x1, float y1, float z1);
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
void BlockUpdate(Block block, struct Level * level, int x, int y, int z, RandomGenerator * random);
void BlockSpawnBreakParticles(Block block, struct Level * level, int x, int y, int z, ParticleManager * particles);
LiquidType BlockGetLiquidType(Block block);
void BlockOnNeighborChanged(Block block, struct Level * level, int x, int y, int z, BlockType tile);
void BlockOnPlaced(Block block, struct Level * level, int x, int y, int z);
int BlockGetTickDelay(Block block);
void BlockOnAdded(Block block, struct Level * level, int x, int y, int z);
void BlockOnRemoved(Block block, struct Level * level, int x, int y, int z);
void BlockRenderPreview(Block block);
bool BlockCanExplode(Block block);
MovingObjectPosition BlockClip(Block block, int x, int y, int z, Vector3D v0, Vector3D v1);
void BlockExplode(Block block, struct Level * level, int x, int y, int z);
bool BlockRender(Block block, struct Level * level, int x, int y, int z);
int BlockGetRenderPass(Block block);
void BlockDestroy(Block block);

extern struct Blocks {
	Block table[256];
	bool physics[256];
	bool liquid[256];
} Blocks;

void BlocksInitialize(void);
void BlocksDeinitialize(void);
