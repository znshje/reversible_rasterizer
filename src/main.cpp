//#ifdef USE_OFFSCREEN_RENDERER
//#include "src/offscreen_renderer.h"
//#else
//#include "src/renderer.h"
//#endif
#include "api.h"
#include "src/model_loader.h"
#include "src/mesh.hpp"
//#include "src/shader.h"

int main(int argc, char** argv) {
//    Renderer* renderer = new Renderer();
//    if (renderer->init_gl_wnd_program() != 0) {
//        renderer->destroy();
//        free(renderer);
//    }
//    Shader shader("src/shaders/trivert.glsl", "src/shaders/trifrag.glsl");
//
//    renderer->set_width(1278);
//    renderer->set_height(798);
//    renderer->set_camera(
//            {-8.428378835009243f, 0.6499701799917779f, 2.400878313047228f},
//            {1.7015814557303426f, 21.984535530826733f, 148.37043107723093f},
//            13.0,
//            0.0001,
//            10000
//    );
//
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

    render(mesh, config);
//    mesh.transform_center();
//
//    renderer->init_scene();
//    mesh.setup_tri_mesh(1278, 798);
//    renderer->render_mesh(mesh, shader);
//    std::vector<std::vector<int>> id_map = renderer->read_triangle_id(mesh);
//
//    mesh.destroy();
//    renderer->destroy();
    return 0;
}
