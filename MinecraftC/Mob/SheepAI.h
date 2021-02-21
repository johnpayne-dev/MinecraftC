#pragma once
#include "AI/BasicAI.h"
#include "Sheep.h"

typedef BasicAI SheepAI;

typedef struct SheepAIData
{
	Sheep Parent;
} * SheepAIData;

SheepAI SheepAICreate(Sheep parent);
void SheepAIUpdate(SheepAI ai);
void SheepAIDestroy(SheepAI ai);
