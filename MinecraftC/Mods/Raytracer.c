#if MINECRAFTC_MODS

#include "Raytracer.h"
#include "Matrix.h"
#include "../Utilities/OpenGL.h"
#include "../Level/Level.h"
#include "../Utilities/Log.h"
#include "../../Resources/Shaders/Raytracer.h"
#include "../../Resources/Shaders/DistanceField.h"

struct OctreeRenderer Raytracer = { 0 };

bool RaytracerInitialize(TextureManager * textures, Level * level, int width, int height) {
	Raytracer.width = width;
	Raytracer.height = height;
	Raytracer.textures = textures;
	Raytracer.level = level;
	glGenTextures(1, &Raytracer.textureID);
	glBindTexture(GL_TEXTURE_2D, Raytracer.textureID);
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
	if (clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &Raytracer.device, NULL) == CL_DEVICE_NOT_FOUND) {
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
	Raytracer.context = clCreateContext(properties, 1, &Raytracer.device, NULL, NULL, &error);
	if (error < 0) {
		LogError("Failed to create context: %i\n", error);
		return false;
	}
	
	Raytracer.raytracer = clCreateProgramWithSource(Raytracer.context, 1, &(const char *){ (const char *)Resource_Shaders_Raytracer }, &(size_t){ sizeof(Resource_Shaders_Raytracer) }, &error);
	if (error < 0) {
		LogError("Failed to create shader program: %i\n", error);
		return false;
	}
	Raytracer.distanceFieldShader = clCreateProgramWithSource(Raytracer.context, 1, &(const char *){ (const char *)Resource_Shaders_DistanceField }, &(size_t){ sizeof(Resource_Shaders_DistanceField) }, &error);
	if (error < 0) {
		LogError("Failed to create shader program: %i\n", error);
		return false;
	}

	error = clBuildProgram(Raytracer.raytracer, 0, NULL, NULL, NULL, NULL);
	if (error < 0) {
		size_t logSize;
		clGetProgramBuildInfo(Raytracer.raytracer, Raytracer.device, CL_PROGRAM_BUILD_LOG, 0, NULL, &logSize);
		char * log = malloc(logSize);
		clGetProgramBuildInfo(Raytracer.raytracer, Raytracer.device, CL_PROGRAM_BUILD_LOG, logSize, log, NULL);
		LogError("Failed to compile shader program: %s\n", log);
		free(log);
		return false;
	}
	
	error = clBuildProgram(Raytracer.distanceFieldShader, 0, NULL, NULL, NULL, NULL);
	if (error < 0) {
		size_t logSize;
		clGetProgramBuildInfo(Raytracer.distanceFieldShader, Raytracer.device, CL_PROGRAM_BUILD_LOG, 0, NULL, &logSize);
		char * log = malloc(logSize);
		clGetProgramBuildInfo(Raytracer.distanceFieldShader, Raytracer.device, CL_PROGRAM_BUILD_LOG, logSize, log, NULL);
		LogError("Failed to compile shader program: %s\n", log);
		free(log);
		return false;
	}

	Raytracer.queue = clCreateCommandQueue(Raytracer.context, Raytracer.device, 0, &error);
	if (error < 0) {
		LogError("Failed to create command queue: %i\n", error);
		return false;
	}
	Raytracer.traceKernel = clCreateKernel(Raytracer.raytracer, "trace", &error);
	if (error < 0) {
		LogError("Failed to create kernel: %i\n", error);
		return false;
	}
	Raytracer.iteratekernel = clCreateKernel(Raytracer.distanceFieldShader, "iterate", &error);
	if (error < 0) {
		LogError("Failed to create kernel: %i\n", error);
		return false;
	}
	
	Raytracer.blockBuffer = clCreateBuffer(Raytracer.context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, level->width * level->height * level->depth, level->blocks, &error);
	if (error < 0) {
		LogFatal("Failed to create block buffer: %i\n", error);
	}

	Raytracer.outputTexture = clCreateFromGLTexture(Raytracer.context, CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0, Raytracer.textureID, &error);
	if (error < 0) {
		LogError("Failed to create texture buffer: %i\n", error);
		return false;
	}
	Raytracer.terrainTexture = clCreateFromGLTexture(Raytracer.context, CL_MEM_READ_ONLY, GL_TEXTURE_2D, 0, TextureManagerLoad(textures, "Terrain.png"), &error);
	if (error < 0) {
		LogError("Failed to create texture buffer: %i\n", error);
		return false;
	}
	
	Raytracer.distanceFieldBuffer = clCreateBuffer(Raytracer.context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, level->width * level->height * level->depth, level->distanceField, &error);
	if (error < 0) {
		LogError("Failed to create buffer: %i\n", error);
		return false;
	}
	
	error |= clSetKernelArg(Raytracer.iteratekernel, 0, sizeof(cl_mem), &Raytracer.distanceFieldBuffer);
	error |= clSetKernelArg(Raytracer.iteratekernel, 2, sizeof(int), &level->width);
	error |= clSetKernelArg(Raytracer.traceKernel, 0, sizeof(int), &level->width);
	error |= clSetKernelArg(Raytracer.traceKernel, 1, sizeof(cl_mem), &Raytracer.distanceFieldBuffer);
	error |= clSetKernelArg(Raytracer.traceKernel, 2, sizeof(cl_mem), &Raytracer.blockBuffer);
	error |= clSetKernelArg(Raytracer.traceKernel, 3, sizeof(cl_mem), &Raytracer.outputTexture);
	error |= clSetKernelArg(Raytracer.traceKernel, 4, sizeof(int), &Raytracer.width);
	error |= clSetKernelArg(Raytracer.traceKernel, 5, sizeof(int), &Raytracer.height);
	error |= clSetKernelArg(Raytracer.traceKernel, 7, sizeof(cl_mem), &Raytracer.terrainTexture);
	if (error < 0) {
		LogError("Failed to set kernel arguments: %i\n", error);
		return false;
	}
	return true;
}

void RaytracerReload() {
	clFinish(Raytracer.queue);
	clReleaseMemObject(Raytracer.distanceFieldBuffer);
	clReleaseMemObject(Raytracer.blockBuffer);
	int error;
	Raytracer.distanceFieldBuffer = clCreateBuffer(Raytracer.context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, Raytracer.level->width * Raytracer.level->height * Raytracer.level->depth, Raytracer.level->distanceField, &error);
	if (error < 0) {
		LogFatal("Failed to create buffer: %i\n", error);
	}
	Raytracer.blockBuffer = clCreateBuffer(Raytracer.context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, Raytracer.level->width * Raytracer.level->height * Raytracer.level->depth, Raytracer.level->blocks, &error);
	if (error < 0) {
		LogFatal("Failed to create block buffer: %i\n", error);
	}
	error |= clSetKernelArg(Raytracer.iteratekernel, 0, sizeof(cl_mem), &Raytracer.distanceFieldBuffer);
	error |= clSetKernelArg(Raytracer.iteratekernel, 2, sizeof(int), &Raytracer.level->width);
	error |= clSetKernelArg(Raytracer.traceKernel, 0, sizeof(int), &Raytracer.level->width);
	error |= clSetKernelArg(Raytracer.traceKernel, 1, sizeof(cl_mem), &Raytracer.distanceFieldBuffer);
	error |= clSetKernelArg(Raytracer.traceKernel, 2, sizeof(cl_mem), &Raytracer.blockBuffer);
	if (error < 0) {
		LogError("Failed to set kernel arguments: %i\n", error);
		return false;
	}
}

void RaytracerEnqueue(float dt, float time, bool doBobbing) {
	Player * player = Raytracer.level->player;
	float a = (player->xRotO + (player->xRot - player->xRot) * dt) * M_PI / 180.0;
	float b = (180.0 - (player->yRotO + (player->yRot - player->yRotO) * dt)) * M_PI / 180.0;
	float x = player->xo + (player->x - player->xo) * dt;
	float y = player->yo + (player->y - player->yo) * dt;
	float z = player->zo + (player->z - player->zo) * dt;
	Matrix4x4 camera = Matrix4x4Multiply(Matrix4x4FromTranslate(x, y, z), Matrix4x4FromEulerAngles(b, a, 0.0));
	if (doBobbing) {
		float walk = player->walkDistance - player->oldWalkDistance;
		walk = player->walkDistance + walk * dt;
		float bob = (player->player.oldBobbing + (player->player.bobbing - player->player.oldBobbing) * dt) * M_PI / 180.0f;
		float tilt = (player->player.oldTilt + (player->player.tilt - player->player.oldTilt) * dt) * M_PI / 180.0f;
		Matrix4x4 bobbing = Matrix4x4Identity;
		bobbing = Matrix4x4Multiply(Matrix4x4FromAxisAngle(0.0, 0.0, 1.0, -sin(walk * M_PI) * bob * 3.0), bobbing);
		bobbing = Matrix4x4Multiply(Matrix4x4FromAxisAngle(1.0, 0.0, 0.0, -fabs(cos(walk * M_PI + 0.2) * bob) * 5.0), bobbing);
		bobbing = Matrix4x4Multiply(Matrix4x4FromAxisAngle(1.0, 0.0, 0.0, -tilt), bobbing);
		bobbing = Matrix4x4Multiply(Matrix4x4FromTranslate(-sin(walk * M_PI) * bob * 0.5, fabs(cos(walk * M_PI) * bob), 0.0), bobbing);
		camera = Matrix4x4Multiply(camera, bobbing);
	}
	
	glFinish();
	int error = clSetKernelArg(Raytracer.iteratekernel, 1, sizeof(uint8_t), &(uint8_t){ Raytracer.iteration++ });
	error |= clEnqueueNDRangeKernel(Raytracer.queue, Raytracer.iteratekernel, 3, NULL, (size_t []){ Raytracer.level->width, Raytracer.level->depth, Raytracer.level->height }, NULL, 0, NULL, NULL);
	if (error < 0) {
		LogFatal("Failed to enqueue octree renderer: %i\n", error);
	}
	if (Raytracer.iteration == 64) {
		Raytracer.iteration = 0;
	}
	
	error = clSetKernelArg(Raytracer.traceKernel, 6, sizeof(camera), &camera);
	error |= clSetKernelArg(Raytracer.traceKernel, 8, sizeof(int), &(int){ EntityIsUnderWater(player) });
	error |= clSetKernelArg(Raytracer.traceKernel, 9, sizeof(float), &time);
	if (error < 0) {
		LogFatal("Failed to set kernel argument: %i\n", error);
	}
	error = clEnqueueAcquireGLObjects(Raytracer.queue, 2, (cl_mem[]){ Raytracer.outputTexture, Raytracer.terrainTexture }, 0, NULL, NULL);
	if (error < 0) {
		LogFatal("Failed to aquire gl texture: %i\n", error);
	}
	error = clEnqueueNDRangeKernel(Raytracer.queue, Raytracer.traceKernel, 2, NULL, (size_t[]){ Raytracer.width, Raytracer.height }, NULL, 0, NULL, NULL);
	if (error < 0) {
		LogFatal("Failed to enqueue octree renderer: %i\n", error);
	}
	error = clEnqueueReleaseGLObjects(Raytracer.queue, 2, (cl_mem[]){ Raytracer.outputTexture, Raytracer.terrainTexture }, 0, NULL, NULL);
	if (error < 0) {
		LogFatal("Failed to release gl texture: %i\n", error);
	}
	clFinish(Raytracer.queue);
}

void RaytracerResize(int width, int height) {
	clReleaseMemObject(Raytracer.outputTexture);
	glDeleteTextures(1, &Raytracer.textureID);
	Raytracer.width = width;
	Raytracer.height = height;
	glGenTextures(1, &Raytracer.textureID);
	glBindTexture(GL_TEXTURE_2D, Raytracer.textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	int error;
	Raytracer.outputTexture = clCreateFromGLTexture(Raytracer.context, CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0, Raytracer.textureID, &error);
	if (error < 0) {
		LogFatal("Failed to create texture buffer: %i\n", error);
	}
	error = clSetKernelArg(Raytracer.traceKernel, 3, sizeof(cl_mem), &Raytracer.outputTexture);
	error |= clSetKernelArg(Raytracer.traceKernel, 4, sizeof(int), &Raytracer.width);
	error |= clSetKernelArg(Raytracer.traceKernel, 5, sizeof(int), &Raytracer.height);
	if (error < 0) {
		LogFatal("Failed to set kernel arguments: %i\n", error);
	}
}

void RaytracerDestroy() {
	glFinish();
	clFinish(Raytracer.queue);
	clReleaseMemObject(Raytracer.distanceFieldBuffer);
	clReleaseMemObject(Raytracer.terrainTexture);
	clReleaseMemObject(Raytracer.outputTexture);
	clReleaseMemObject(Raytracer.blockBuffer);
	clReleaseCommandQueue(Raytracer.queue);
	clReleaseKernel(Raytracer.iteratekernel);
	clReleaseProgram(Raytracer.distanceFieldShader);
	clReleaseKernel(Raytracer.traceKernel);
	clReleaseProgram(Raytracer.raytracer);
	clReleaseContext(Raytracer.context);
	clReleaseDevice(Raytracer.device);
	glDeleteTextures(1, &Raytracer.textureID);
	Raytracer = (struct OctreeRenderer){ 0 };
}

#endif
