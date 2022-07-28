#if MINECRAFTC_MODS

#include "OctreeRenderer.h"
#include "../Utilities/OpenGL.h"
#include "../Level/Level.h"
#include "../Utilities/Log.h"
#include "../../Resources/Shaders/Raytracer.h"

struct OctreeRenderer OctreeRenderer = { 0 };

bool OctreeRendererInitialize(int width, int height) {
	OctreeRenderer.width = width;
	OctreeRenderer.height = height;
	glGenTextures(1, &OctreeRenderer.textureID);
	glBindTexture(GL_TEXTURE_2D, OctreeRenderer.textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	cl_platform_id platform;
	if (clGetPlatformIDs(1, &platform, NULL) < 0) {
		LogError("Couldn't find a suitable platform for OpenCL\n");
		return false;
	}
	if (clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &OctreeRenderer.device, NULL) == CL_DEVICE_NOT_FOUND) {
		LogError("No supported GPU found\n");
		return false;
	}
	
	cl_context_properties properties[] = {
#ifdef __APPLE__
		CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
		(cl_context_properties)CGLGetShareGroup(CGLGetCurrentContext()),
#elif defined(_WIN32)
		CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
		CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
		CL_CONTEXT_PLATFORM, (cl_context_properties)platform,
#elif defined(__linux__)
		CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
		CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
		CL_CONTEXT_PLATFORM, (cl_context_properties)platform,
#endif
		0,
	};

	int error;
	OctreeRenderer.context = clCreateContext(properties, 1, &OctreeRenderer.device, NULL, NULL, &error);
	if (error < 0) {
		LogError("Failed to create context: %i\n", error);
		return false;
	}
	
	OctreeRenderer.shader = clCreateProgramWithSource(OctreeRenderer.context, 1, &(const char *){ (const char *)Resource_Shaders_Raytrace }, &(size_t){ sizeof(Resource_Shaders_Raytrace) }, &error);
	if (error < 0) {
		LogError("Failed to create shader program: %i\n", error);
		return false;
	}

	error = clBuildProgram(OctreeRenderer.shader, 0, NULL, NULL, NULL, NULL);
	if (error < 0) {
		size_t logSize;
		clGetProgramBuildInfo(OctreeRenderer.shader, OctreeRenderer.device, CL_PROGRAM_BUILD_LOG, 0, NULL, &logSize);
		char * log = malloc(logSize);
		clGetProgramBuildInfo(OctreeRenderer.shader, OctreeRenderer.device, CL_PROGRAM_BUILD_LOG, logSize, log, NULL);
		LogError("Failed to compile shader program: %s\n", log);
		free(log);
		return false;
	}

	OctreeRenderer.queue = clCreateCommandQueue(OctreeRenderer.context, OctreeRenderer.device, 0, &error);
	if (error < 0) {
		LogError("Failed to create command queue: %i\n", error);
		return false;
	}
	OctreeRenderer.kernel = clCreateKernel(OctreeRenderer.shader, "trace", &error);
	if (error < 0) {
		LogError("Failed to create kernel: %i\n", error);
		return false;
	}

	OctreeRenderer.textureBuffer = clCreateFromGLTexture(OctreeRenderer.context, CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0, OctreeRenderer.textureID, &error);
	if (error < 0) { LogFatal("Failed to create texture buffer: %i %i\n", error, CL_INVALID_GL_OBJECT); }
	error = clSetKernelArg(OctreeRenderer.kernel, 3, sizeof(cl_mem), &OctreeRenderer.textureBuffer);
	if (error < 0) {
		LogError("Failed to set kernel arguments: %i\n", error);
		return false;
	}
	return true;
}

void OctreeRendererSetOctree(Octree * tree) {
	OctreeRenderer.octree = tree;

	int error;
	OctreeRenderer.octreeBuffer = clCreateBuffer(OctreeRenderer.context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, tree->maskCount, tree->masks, &error);
	if (error < 0) {
		LogFatal("Failed to create octree buffer: %i\n", error);
	}
	OctreeRenderer.blockBuffer = clCreateBuffer(OctreeRenderer.context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, tree->level->width * tree->level->height * tree->level->depth, tree->level->blocks, &error);
	if (error < 0) {
		LogFatal("Failed to create block buffer: %i\n", error);
	}

	error = clSetKernelArg(OctreeRenderer.kernel, 0, sizeof(unsigned int), &tree->depth);
	error |= clSetKernelArg(OctreeRenderer.kernel, 1, sizeof(cl_mem), &OctreeRenderer.octreeBuffer);
	error |= clSetKernelArg(OctreeRenderer.kernel, 2, sizeof(cl_mem), &OctreeRenderer.blockBuffer);
	if (error < 0) {
		LogFatal("Failed to set kernel arguments: %i\n", error);
	}
}

void OctreeRendererEnqueue() {
	glFinish();
	Player * player = OctreeRenderer.octree->level->player;
	float a = player->xRot * M_PI / 180.0, b = (180.0 - player->yRot) * M_PI / 180.0;
	float x = player->x, y = player->y, z = player->z;
	float matrix[16] = {
		cosf(b), 0.0, -sinf(b), 0.0,
		sinf(a) * sinf(b), cosf(a), sinf(a) * cosf(b), 0.0,
		cosf(a) * sinf(b), -sinf(a), cosf(a) * cosf(b), 0.0,
		x, y, z, 1.0
	};
	//Matrix4x4 camera = Matrix4x4Multiply(Matrix4x4FromTranslate(player->Position), Matrix4x4FromEulerAngles((float3){ 180.0 - player->Rotation.y, player->Rotation.x, 0.0 } * rad));
	int error = clSetKernelArg(OctreeRenderer.kernel, 4, sizeof(matrix), &matrix);
	if (error < 0) {
		LogFatal("Failed to set kernel argument: %i\n", error);
	}
	error = clEnqueueAcquireGLObjects(OctreeRenderer.queue, 1, &OctreeRenderer.textureBuffer, 0, NULL, NULL);
	if (error < 0) {
		LogFatal("Failed to aquire gl texture: %i\n");
	}
	error = clEnqueueNDRangeKernel(OctreeRenderer.queue, OctreeRenderer.kernel, 2, NULL, (size_t[]){ OctreeRenderer.width, OctreeRenderer.height }, (size_t[]){ 1, 1 }, 0, NULL, NULL);
	if (error < 0) {
		LogFatal("Failed to enqueue octree renderer: %i\n");
	}
	error = clEnqueueReleaseGLObjects(OctreeRenderer.queue, 1, &OctreeRenderer.textureBuffer, 0, NULL, NULL);
	if (error < 0) {
		LogFatal("Failed to release gl texture: %i\n");
	}
	clFinish(OctreeRenderer.queue);
}

void OctreeRendererDestroy() {
	glFinish();
	clFinish(OctreeRenderer.queue);
	clReleaseMemObject(OctreeRenderer.textureBuffer);
	clReleaseMemObject(OctreeRenderer.octreeBuffer);
	clReleaseMemObject(OctreeRenderer.blockBuffer);
	clReleaseKernel(OctreeRenderer.kernel);
	clReleaseCommandQueue(OctreeRenderer.queue);
	clReleaseProgram(OctreeRenderer.shader);
	clReleaseContext(OctreeRenderer.context);
	clReleaseDevice(OctreeRenderer.device);
	glDeleteTextures(1, &OctreeRenderer.textureID);
}

#endif
