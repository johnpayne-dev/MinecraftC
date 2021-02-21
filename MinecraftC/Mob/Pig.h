#pragma once
#include "Mob.h"

typedef Mob Pig;

Pig PigCreate(Level level, float3 pos);
void PigDie(Pig pig, Entity entity);
