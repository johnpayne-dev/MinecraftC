#pragma once
#include "../Entity.h"
#include "../Level/Level.h"

typedef Entity PrimedTNT;

typedef struct PrimedTNTData
{
	float3 Delta;
	int Life;
	bool Defused;
} * PrimedTNTData;

PrimedTNT PrimedTNTCreate(Level level, float3 pos);
void PrimedTNTHurt(PrimedTNT tnt);
bool PrimedTNTIsPickable(PrimedTNT tnt);
void PrimedTNTTick(PrimedTNT tnt);
void PrimedTNTRender(PrimedTNT tnt, TextureManager textures, float t);
void PrimedTNTDestroy(PrimedTNT tnt);
