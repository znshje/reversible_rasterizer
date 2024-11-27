//
// Created by zsj on 24-11-23.
//

#ifndef REVERSIBLE_RASTERIZER_CONFIG_H
#define REVERSIBLE_RASTERIZER_CONFIG_H

#include <vector>

struct CameraConfig {
    int width;
    int height;
    std::vector<float> R;
    std::vector<float> T;
    double zoom;
    double near;
    double far;

    CameraConfig() {
        this->width = 1280;
        this->height = 800;
        this->R = {0.f, 0.f, 0.f};
        this->T = {0.f, 0.f, 0.f};
        this->zoom = 1;
        this->near = 0.0001;
        this->far = 10000;
    }
};

#endif //REVERSIBLE_RASTERIZER_CONFIG_H
