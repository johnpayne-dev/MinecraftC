#pragma once
#include "AI/BasicAttackAI.h"
#include "Creeper.h"

typedef BasicAttackAI CreeperAI;

typedef struct CreeperAIData
{
	Creeper Parent;
} * CreeperAIData;

CreeperAI CreeperAICreate(Creeper parent);
bool CreeperAIAttack(CreeperAI ai, Entity entity);
void CreeperAIBeforeRemove(CreeperAI ai);
void CreeperAIDestroy(CreeperAI ai);
