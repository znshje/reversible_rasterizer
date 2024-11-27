//
// Created by zsj on 24-11-26.
//

#ifndef REVERSIBLE_RASTERIZER_API_H
#define REVERSIBLE_RASTERIZER_API_H

#include "config.h"
#include "mesh.hpp"

std::vector<std::vector<int>> render(Mesh mesh, CameraConfig config);

#endif //REVERSIBLE_RASTERIZER_API_H
