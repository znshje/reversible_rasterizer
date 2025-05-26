#include "api.h"
#include "model_loader.h"
#include "mesh.hpp"

int main(int argc, char** argv) {
    Renderer* renderer = new Renderer();
    if (renderer->init_gl_wnd_program() != 0) {
        renderer->destroy();
        free(renderer);
    }
    ModelLoader* model_loader = new ModelLoader(argv[1]);

    Mesh mesh(model_loader->get_vertices(), model_loader->get_normals(), model_loader->get_faces(), model_loader->get_vertex_colors());
    CameraConfig config;
    config.width = 1278;
    config.height = 798;
    config.R = {-8.428378835009243, 0.6499701799917779, 2.400878313047228};
    config.T = {1.7015814557303426, 21.984535530826733, 148.37043107723093};
    config.zoom = 13;
    config.near = 0.0001;
    config.far = 10000;

    render(renderer, mesh, config);
    end(renderer);
    return 0;
}
