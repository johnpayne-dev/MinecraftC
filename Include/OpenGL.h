#pragma once

#if defined(__APPLE__)
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#elif defined(_WIN32)
	#include <Windows.h>
	#include <gl/GL.h>
	#include <gl/GLU.h>
#elif defined(__linux)
#endif
