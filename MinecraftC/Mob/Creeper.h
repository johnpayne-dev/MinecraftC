#pragma once
#include "Mob.h"

typedef Mob Creeper;

Creeper CreeperCreate(Level level, float3 pos);
float CreeperGetBrightness(Creeper creeper, float t);
