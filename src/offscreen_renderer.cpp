//
// Created by zsj on 24-11-23.
//

#include "offscreen_renderer.h"
#include <Eigen/Eigen>
#include <utility>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int Renderer::init_gl_wnd_program() {
    OSMesaContext context = OSMesaCreateContext(OSMESA_RGBA, NULL);
    if (!context) {
        std::cerr << "Failed to create OSMesa context" << std::endl;
        return -1;
    }

    // 2. 创建一个帧缓冲区（PBuffer）
    buffer = new GLubyte[WIDTH * HEIGHT * 4];  // RGBA buffer
    if (!OSMesaMakeCurrent(context, buffer, GL_UNSIGNED_BYTE, WIDTH, HEIGHT)) {
        std::cerr << "Failed to make OSMesa current" << std::endl;
        return -1;
    }

    // 3. 使用 GLAD2 加载 OpenGL 函数
    if (!gladLoadGLLoader((GLADloadproc) OSMesaGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    return 0;
}

void Renderer::destroy() {
    delete[] buffer;
}

void Renderer::init_scene() {
    glViewport(0, 0, width, height);

    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    /* Swap front and back buffers */
//    glfwSwapBuffers(window);
}

void Renderer::render_mesh(std::vector<std::vector<double>> vertices, std::vector<std::vector<uint32_t>> faces) {
    Eigen::MatrixX3d mVertices;
    mVertices.resize((long) vertices.size(), 3);
    for (int i = 0; i < vertices.size(); i++) {
        for (int j = 0; j < 3; j++) {
            mVertices(i, j) = vertices[i][j];
        }
    }

//    glEnable(GL_DEPTH_TEST)
    glClearColor(0.4f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

//    glBegin(GL_TRIANGLES);
//    glColor3f(0.5f, 0.1f, 0.2f);
//    for (auto &face: faces) {
//        for (unsigned int vi: face) {
//            glVertex3d(mVertices(vi, 0), mVertices(vi, 1), mVertices(vi, 2));
//        }
//    }
//
//    glEnd();
//    glfwSwapBuffers(window);
}

void Renderer::loop() {

}

void Renderer::set_camera(std::vector<float> r, std::vector<float> t, double z, double n, double f) {
    camera_rotate = std::move(r);
    camera_translate = std::move(t);
    zoom = z;
    near = n;
    far = f;

    proj = glm::ortho(-width / 2.0 / zoom, width / 2.0 / zoom, -height / 2.0 / zoom, height / 2.0 / zoom, near, far);
}

void Renderer::set_height(int h) {
    height = h;
}

void Renderer::set_width(int w) {
    width = w;
}

void Renderer::render_mesh(Mesh mesh, Shader shader) {
    shader.Use();

    glm::mat4 rot(1);
    rot = glm::rotate(rot, glm::radians(camera_rotate[0]), glm::vec3(1, 0, 0));
    rot = glm::rotate(rot, glm::radians(camera_rotate[1]), glm::vec3(0, 1, 0));
    rot = glm::rotate(rot, glm::radians(camera_rotate[2]), glm::vec3(0, 0, 1));
    rot = glm::inverse(rot);
    glm::mat4 trans(1);
    trans = glm::translate(trans, glm::vec3(-camera_translate[0], -camera_translate[1], -camera_translate[2]));
    trans = rot * trans;

    GLint modelLoc = glGetUniformLocation(shader.Program, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));

    GLint viewLoc = glGetUniformLocation(shader.Program, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(trans));

    GLint projLoc = glGetUniformLocation(shader.Program, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

    glm::vec3 lightPos = {0, 0, 100.f};
    glUniform3f(glGetUniformLocation(shader.Program, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(glGetUniformLocation(shader.Program, "lightColor"), 1.0, 1.0, 1.0);
    glUniform3f(glGetUniformLocation(shader.Program, "viewPos"), camera_translate[0], camera_translate[1],
                camera_translate[2]);

    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mesh.tri_draw(width, height);
//    glfwSwapBuffers(window);
}

// 读取像素的三角形 ID
std::vector<std::vector<int>> Renderer::read_triangle_id(Mesh mesh) {
    std::vector<std::vector<int>> triangleIDs(height, std::vector<int>(width));
    GLint pixelData[width * height];
    glBindTexture(GL_TEXTURE_2D, mesh.idTexture);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED_INTEGER, GL_INT, pixelData);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // OpenGL 的原点在左下角，需要翻转 Y 轴存储数据
            triangleIDs[height - 1 - y][x] = pixelData[y * width + x];
        }
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    return triangleIDs;
}