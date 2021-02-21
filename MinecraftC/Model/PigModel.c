#include "PigModel.h"

PigModel PigModelCreate()
{
	Model model = AnimalModelCreate(6, 0.0);
	model->Type = ModelTypePig;
	return model;
}
