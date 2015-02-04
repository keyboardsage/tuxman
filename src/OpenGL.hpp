#ifndef OPENGL_HPP
#define OPENGL_HPP

#ifdef _WIN32
#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#elif linux
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#endif
