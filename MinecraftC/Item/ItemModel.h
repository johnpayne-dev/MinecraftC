#pragma once
#include "../Model/ModelPart.h"

typedef struct ItemModel
{
	ModelPart Part;
} * ItemModel;

ItemModel ItemModelCreate(int texID);
void ItemModelGenerateList(ItemModel model);
void ItemModelDestroy(ItemModel model);
