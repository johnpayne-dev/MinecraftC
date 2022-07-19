#pragma once
#include <stdbool.h>
#include "../Render/TextureManager.h"

struct Level;

typedef struct Entity PrimedTNT;

typedef struct PrimedTNTData {
	float xd, yd, zd;
	int life;
	bool defused;
} PrimedTNTData;

void PrimedTNTCreate(PrimedTNT * tnt, struct Level * level, float x, float y, float z);
void PrimedTNTOnHit(PrimedTNT * tnt);
bool PrimedTNTIsPickable(PrimedTNT * tnt);
void PrimedTNTTick(PrimedTNT * tnt);
void PrimedTNTRender(PrimedTNT * tnt, TextureManager * textures, float t);
