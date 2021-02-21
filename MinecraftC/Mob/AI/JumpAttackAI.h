#pragma once
#include "BasicAttackAI.h"

typedef BasicAttackAI JumpAttackAI;

JumpAttackAI JumpAttackAICreate(void);
void JumpAttackAIJumpFromGround(JumpAttackAI ai);
