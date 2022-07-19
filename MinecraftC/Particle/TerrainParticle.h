#pragma once
#include "Particle.h"
#include "../Level/Tile/Block.h"

typedef Particle TerrainParticle;

void TerrainParticleCreate(TerrainParticle * particle, struct Level * level, float x, float y, float z, float xd, float yd, float zd, Block block);
int TerrainParticleGetTexture(TerrainParticle * particle);
void TerrainParticleRender(TerrainParticle * particle, float t, float x, float y, float z, float v6, float v7);
