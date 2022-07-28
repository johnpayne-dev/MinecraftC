#pragma once

#if defined(__APPLE__)
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <OpenGL/CGLDevice.h>
	#include <OpenGL/CGLTypes.h>
	#include <OpenGL/CGLCurrent.h>
#elif defined(_WIN32)
	#include <Windows.h>
	#include <gl/GL.h>
	#include <gl/GLU.h>
#elif defined(__linux)
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif
