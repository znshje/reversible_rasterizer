//
// Created by zsj on 24-11-25.
//

#ifndef REVERSIBLE_RASTERIZER_SHADER_H
#define REVERSIBLE_RASTERIZER_SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "glspec.h"

class Shader
{
public:
    Shader();

// 程序ID
    GLuint Program;
    // 构造器读取并构建着色器
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    // 使用程序
    void Use();
};

#endif //REVERSIBLE_RASTERIZER_SHADER_H
