#pragma once
#if MINECRAFTC_MODS

#include "../Utilities/OpenCL.h"
#include "Octree.h"

struct OctreeRenderer {
	int width, height;
	cl_device_id device;
	cl_context context;
	cl_program shader;
	cl_kernel kernel;
	cl_command_queue queue;
	cl_mem octreeBuffer, blockBuffer;
	cl_mem textureBuffer;
	uint32_t textureID;
	Octree * octree;
} extern OctreeRenderer;

bool OctreeRendererInitialize(int width, int height);
void OctreeRendererSetOctree(Octree * tree);
void OctreeRendererEnqueue(void);
void OctreeRendererDestroy(void);

#endif
