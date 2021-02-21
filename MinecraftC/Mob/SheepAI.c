#include "SheepAI.h"

SheepAI SheepAICreate(Sheep parent)
{
	AI ai = BasicAICreate();
	BasicAIData basicAI = ai->TypeData;
	basicAI->Type = BasicAITypeSheep;
	SheepAIData sheep = MemoryAllocate(sizeof(struct SheepAIData));
	*sheep = (struct SheepAIData){ .Parent = parent, };
	basicAI->TypeData = sheep;
	return ai;
}

void SheepAIUpdate(SheepAI ai)
{
	BasicAIData basicAI = ai->TypeData;
	SheepAIData this = basicAI->TypeData;
	SheepData sheep = ((MobData)this->Parent->TypeData)->TypeData;
	float s = sin(this->Parent->Rotation.y * rad) * -0.7;
	float c = cos(this->Parent->Rotation.y * rad) * 0.7;
	int3 v = int3f(basicAI->Mob->Position + (float3){ s, -2.0, c });
	if (sheep->Grazing)
	{
		if (LevelGetTile(basicAI->Level, v.x, v.y, v.z) != BlockTypeGrass) { sheep->Grazing = false; }
		else
		{
			if (++sheep->GrazingTime == 60)
			{
				LevelSetTile(basicAI->Level, v.x, v.y, v.z, BlockTypeDirt);
				if (RandomGeneratorIntegerRange(basicAI->Random, 0, 5) == 0) { sheep->HasFur = true; }
			}
			basicAI->XY = zero2f;
			basicAI->Mob->Rotation.x = 40.0 + sheep->GrazingTime / 2 % 2 * 10;
		}
	}
	else
	{
		if (LevelGetTile(basicAI->Level, v.x, v.y, v.z) == BlockTypeGrass)
		{
			sheep->Grazing = true;
			sheep->GrazingTime = 0;
		}
		basicAI->Type = BasicAITypeNone;
		BasicAIUpdate(ai);
		basicAI->Type = BasicAITypeSheep;
	}
}

void SheepAIDestroy(SheepAI ai)
{
	BasicAIData basicAI = ai->TypeData;
	SheepAIData this = basicAI->TypeData;
	MemoryFree(this);
}
