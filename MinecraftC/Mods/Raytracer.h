#pragma once
#if MINECRAFTC_MODS

#include "../Utilities/OpenCL.h"
#include "../Render/TextureManager.h"
#include "../Level/Tile/Block.h"

struct OctreeRenderer {
	int width, height;
	cl_device_id device;
	cl_context context;
	cl_program raytracer;
	cl_kernel traceKernel;
	cl_program distanceFieldShader;
	cl_kernel iteratekernel;
	cl_command_queue queue;
	cl_mem blockBuffer;
	cl_mem outputTexture;
	cl_mem terrainTexture;
	cl_mem distanceFieldBuffer;
	uint8_t iteration;
	uint32_t textureID;
	TextureManager * textures;
	struct Level * level;
} extern Raytracer;

bool RaytracerInitialize(TextureManager * textures, struct Level * level, int width, int height);
void RaytracerReload(void);
void RaytracerEnqueue(float dt, float time, bool doBobbing);
void RaytracerResize(int width, int height);
void RaytracerDestroy(void);

#endif
