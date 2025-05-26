//
// Created by zsj on 24-11-23.
//

#ifndef REVERSIBLE_RASTERIZER_RENDERER_H
#define REVERSIBLE_RASTERIZER_RENDERER_H

#define WIDTH 1278
#define HEIGHT 798

#ifdef USE_OFFSCREEN_RENDERER
#include "glspec.h"
#include <vector>
#include "mesh.hpp"
#include "shader.h"

class Renderer {
public:
    int init_gl_wnd_program();
    void render_mesh(std::vector<std::vector<double>>, std::vector<std::vector<uint32_t>>);
    void render_mesh(Mesh, Shader shader);
    void init_scene();
    void destroy();
    void loop();

    void set_width(int);
    void set_height(int);
    void set_camera(std::vector<float> r, std::vector<float> t, double zoom, double near, double far);

    std::vector<std::vector<int>> read_triangle_id(Mesh mesh);

    Renderer() : width(WIDTH), height(HEIGHT) {
    }

    ~Renderer() {
        if (buffer) {
            std::cout << "Delete:" << buffer << std::endl;
            delete[] buffer;
        }
    }

    GLubyte* buffer;
private:
    int width, height;
    double near, far;
    double zoom;
    std::vector<float> camera_rotate;
    std::vector<float> camera_translate;
    glm::mat4 proj;

    EGLDisplay display;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;
};

#endif
#endif //REVERSIBLE_RASTERIZER_RENDERER_H
