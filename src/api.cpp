//
// Created by zsj on 24-11-26.
//
#include "api.h"

Renderer* init() {
    Renderer* renderer = new Renderer();
    if (renderer->init_gl_wnd_program() != 0) {
        std::cout << "Renderer init failed" << std::endl;
        renderer->destroy();
        return nullptr;
    }
    return renderer;
}

std::vector<std::vector<int>> render_id_map(Renderer* renderer, Mesh mesh, CameraConfig config) {
    Shader shader(0);
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
//    renderer->destroy();
//    delete renderer;
    return id_map;
}

std::vector<std::vector<std::vector<int>>> render(Renderer* renderer, Mesh mesh, CameraConfig config) {
    Shader shader(0);
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
    std::vector<std::vector<std::vector<int>>> img = renderer->read_image(mesh);
    mesh.destroy();
    return img;
}

std::vector<std::vector<std::vector<int>>> render_normal(Renderer* renderer, Mesh mesh, CameraConfig config) {
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
    renderer->render_mesh_normal(mesh);
    std::vector<std::vector<std::vector<int>>> img = renderer->read_image(mesh);
    mesh.destroy();
    return img;
}

std::vector<std::vector<std::vector<int>>> render_depth(Renderer* renderer, Mesh mesh, CameraConfig config) {
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
    renderer->render_mesh_depth(mesh);
    std::vector<std::vector<std::vector<int>>> img = renderer->read_image(mesh);
    mesh.destroy();
    return img;
}

void end(Renderer* renderer) {
//    delete renderer;
}