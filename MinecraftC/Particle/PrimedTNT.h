#pragma once
#include "../Entity.h"
#include "../Level/Level.h"

typedef Entity PrimedTNT;

typedef struct PrimedTNTData {
	float xd, yd, zd;
	int life;
	bool defused;
} * PrimedTNTData;

PrimedTNT PrimedTNTCreate(Level level, float x, float y, float z);
void PrimedTNTOnHit(PrimedTNT tnt);
bool PrimedTNTIsPickable(PrimedTNT tnt);
void PrimedTNTTick(PrimedTNT tnt);
void PrimedTNTRender(PrimedTNT tnt, TextureManager textures, float t);
void PrimedTNTDestroy(PrimedTNT tnt);
