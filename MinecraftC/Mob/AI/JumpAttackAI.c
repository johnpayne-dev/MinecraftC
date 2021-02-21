#include "JumpAttackAI.h"

JumpAttackAI JumpAttackAICreate()
{
	BasicAttackAI ai = BasicAttackAICreate();
	BasicAIData basicAI = ai->TypeData;
	basicAI->RunSpeed *= 0.8;
	BasicAttackAIData attackAI = basicAI->TypeData;
	attackAI->Type = BasicAttackAIJumpAttack;
	return ai;
}

void JumpAttackAIJumpFromGround(JumpAttackAI ai)
{
	BasicAIData basicAI = ai->TypeData;
	if (basicAI->AttackTarget != NULL)
	{
		basicAI->Mob->Delta.xz = zero2f;
		EntityMoveRelative(basicAI->Mob, up2f, 0.6);
		basicAI->Mob->Delta.y = 0.5;
	}
}
