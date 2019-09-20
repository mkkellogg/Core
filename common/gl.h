#pragma once

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES 1
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
//#define MANUAL_2D_SHADOWS 1
#else
#include <GL/gl.h>
#include <GL/glcorearb.h>
#include <GL/glext.h>
#endif

