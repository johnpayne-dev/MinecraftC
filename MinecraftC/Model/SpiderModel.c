#include "SpiderModel.h"
#include "../Utilities/Memory.h"

SpiderModel SpiderModelCreate()
{
	SpiderModel model = ModelCreate();
	SpiderModelData this = MemoryAllocate(sizeof(struct SpiderModelData));
	model->Type = ModelTypeSpider;
	model->TypeData = this;
	this->Head = ModelPartCreate((int2){ 32, 4 });
	ModelPartSetBounds(this->Head, (float3){ -4.0, -4.0, -8.0 }, (int3){ 8, 8, 8 }, 0.0);
	ModelPartSetPosition(this->Head, (float3){ 0.0, 0.0 -3.0 });
	this->Neck = ModelPartCreate((int2){ 0, 0 });
	ModelPartSetBounds(this->Neck, (float3){ -3.0, -3.0, -3.0 }, (int3){ 6, 6, 6 }, 0.0);
	this->Body = ModelPartCreate((int2){ 0, 12 });
	ModelPartSetBounds(this->Body, (float3){ -5.0, -4.0, -6.0 }, (int3){ 10, 8, 12 }, 0.0);
	ModelPartSetPosition(this->Body, (float3){ 0.0, 0.0, 9.0 });
	for (int i = 0; i < 8; i++) { this->Legs[i] = ModelPartCreate((int2){ 18, 0 }); }
	for (int i = 0; i < 8; i++) { ModelPartSetBounds(this->Legs[i], (float3){ i % 2 == 0 ? -15.0 : -1.0, -1.0, -1.0 }, (int3){ 16, 2, 2 }, 0.0); }
	for (int i = 0; i < 8; i++) { ModelPartSetPosition(this->Legs[i], (float3){ i % 2 == 0 ? -4.0 : 4.0, 0.0, 2.0 - i / 2 }); }
	return model;
}

void SpiderModelRender(SpiderModel model, float anim, float t, float run, float2 rot, float offset)
{
	SpiderModelData this = model->TypeData;
	this->Head->Rotation.yx = rot * rad;
	this->Legs[0]->Rotation.yz = (float2){ pi / 4.0, -pi / 4.0 } + (float2){ -cos(anim * 0.6662 * 2.0), fabs(sin(anim * 0.6662)) } * t * 0.4;
	this->Legs[1]->Rotation.yz = (float2){ -pi / 4.0, pi / 4.0 } - (float2){ -cos(anim * 0.6662 * 2.0), fabs(sin(anim * 0.6662)) } * t * 0.4;
	this->Legs[2]->Rotation.yz = (float2){ pi / 8.0, -pi / 4.0 * 0.74 } + (float2){ -cos(anim * 0.6662 * 2.0 + pi), fabs(sin(anim * 0.6662 + pi)) } * t * 0.4;
	this->Legs[3]->Rotation.yz = (float2){ -pi / 8.0, pi / 4.0 * 0.74 } - (float2){ -cos(anim * 0.6662 * 2.0 + pi), fabs(sin(anim * 0.6662 + pi)) } * t * 0.4;
	this->Legs[4]->Rotation.yz = (float2){ -pi / 8.0, -pi / 4.0 * 0.74 } + (float2){ -cos(anim * 0.6662 * 2.0 + pi / 2.0), fabs(sin(anim * 0.6662 + pi / 2.0)) } * t * 0.4;
	this->Legs[5]->Rotation.yz = (float2){ pi / 8.0, pi / 4.0 * 0.74 } - (float2){ -cos(anim * 0.6662 * 2.0 + pi / 2.0), fabs(sin(anim * 0.6662 + pi / 2.0)) } * t * 0.4;
	this->Legs[6]->Rotation.yz = (float2){ -pi / 4.0, -pi / 4.0 } + (float2){ -cos(anim * 0.6662 * 2.0 + 1.5 * pi), fabs(sin(anim * 0.6662 + 1.5 * pi)) } * t * 0.4;
	this->Legs[7]->Rotation.yz = (float2){ pi / 4.0, pi / 4.0 } - (float2){ -cos(anim * 0.6662 * 2.0 + 1.5 * pi), fabs(sin(anim * 0.6662 + 1.5 * pi)) } * t * 0.4;
	ModelPartRender(this->Head, offset);
	ModelPartRender(this->Neck, offset);
	ModelPartRender(this->Body, offset);
	for (int i = 0; i < 8; i++) { ModelPartRender(this->Legs[i], offset); }
}

void SpiderModelDestroy(SpiderModel model)
{
	SpiderModelData this = model->TypeData;
	ModelPartDestroy(this->Head);
	ModelPartDestroy(this->Neck);
	ModelPartDestroy(this->Body);
	for (int i = 0; i < 8; i++) { ModelPartDestroy(this->Legs[i]); }
	MemoryFree(this);
}
