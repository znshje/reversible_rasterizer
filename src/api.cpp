//
// Created by zsj on 24-11-26.
//
#include "api.h"
#ifdef USE_OFFSCREEN_RENDERER
#include "offscreen_renderer.h"
#else
#include "renderer.h"
#endif

std::vector<std::vector<int>> render(Mesh mesh, CameraConfig config) {
    Renderer* renderer = new Renderer();
    if (renderer->init_gl_wnd_program() != 0) {
        renderer->destroy();
        free(renderer);
    }
    Shader shader;
    renderer->set_width(config.width);
    renderer->set_height(config.height);
    renderer->set_camera(
            config.R,
            config.T,
            config.zoom,
            config.near,
            config.far
    );
    mesh.transform_center();

    renderer->init_scene();
    mesh.setup_tri_mesh(config.width, config.height);
    renderer->render_mesh(mesh, shader);
    std::vector<std::vector<int>> id_map = renderer->read_triangle_id(mesh);
    mesh.destroy();
    renderer->destroy();
    return id_map;
}