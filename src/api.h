//
// Created by zsj on 24-11-26.
//

#ifndef REVERSIBLE_RASTERIZER_API_H
#define REVERSIBLE_RASTERIZER_API_H

#ifdef USE_OFFSCREEN_RENDERER
#include "offscreen_renderer.h"
#else
#include "renderer.h"
#endif

#include "config.h"
#include "mesh.hpp"

Renderer* init();

std::vector<std::vector<int>> render_id_map(Renderer* renderer, Mesh mesh, CameraConfig config);

std::vector<std::vector<std::vector<int>>> render(Renderer* renderer, Mesh mesh, CameraConfig config);

std::vector<std::vector<std::vector<int>>> render_normal(Renderer* renderer, Mesh mesh, CameraConfig config);

std::vector<std::vector<std::vector<int>>> render_depth(Renderer* renderer, Mesh mesh, CameraConfig config);

void end(Renderer* renderer);

#endif //REVERSIBLE_RASTERIZER_API_H
