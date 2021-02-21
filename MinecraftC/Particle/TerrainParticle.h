#pragma once
#include "Particle.h"
#include "../Level/Tile/Block.h"

typedef Particle TerrainParticle;

TerrainParticle TerrainParticleCreate(struct Level * level, float3 pos, float3 vel, Block block);
int TerrainParticleGetTexture(TerrainParticle particle);
void TerrainParticleRender(TerrainParticle particle, float t, float3 v1, float v6, float v7);
