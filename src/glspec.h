//
// Created by zsj on 24-11-27.
//

#ifndef REVERSIBLE_RASTERIZER_GLSPEC_H
#define REVERSIBLE_RASTERIZER_GLSPEC_H

#ifdef USE_OFFSCREEN_RENDERER
#include <glad/gl.h>
#ifndef APIENTRY
#define APIENTRY GLAPIENTRY
#endif
#include <GL/osmesa.h>
#else
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#endif

#endif //REVERSIBLE_RASTERIZER_GLSPEC_H
