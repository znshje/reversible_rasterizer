//
// Created by zsj on 24-11-25.
//
#include "shader.h"

Shader::Shader() {
    std::string vertexCode = "#version 450\n"
                             "layout (location = 0) in vec3 position;\n"
                             "layout (location = 1) in vec3 normal;\n"
                             "layout (location = 2) in vec4 color;\n"
                             "layout (location = 3) in int triangleId;\n"
                             "\n"
                             "out vec4 VertexColor;\n"
                             "out vec3 FragPos;\n"
                             "out vec3 Normal;\n"
                             "out flat int TriangleId;\n"
                             "\n"
                             "uniform mat4 model;\n"
                             "uniform mat4 view;\n"
                             "uniform mat4 projection;\n"
                             "\n"
                             "void main()\n"
                             "{\n"
                             "    gl_Position = projection * view * model * vec4(position, 1.0);\n"
                             "    VertexColor = color;\n"
                             "    FragPos = vec3(model * vec4(position, 1.0f));\n"
                             "    Normal = normal;\n"
                             "    TriangleId = triangleId;\n"
                             "}";

    std::string fragmentCode = "#version 450\n"
                               "in vec4 VertexColor; // 从顶点着色器传来的输入变量（名称相同、类型相同）\n"
                               "in vec3 FragPos;\n"
                               "in vec3 Normal;\n"
                               "in flat int TriangleId;\n"
                               "\n"
                               "uniform vec3 lightPos;\n"
                               "uniform vec3 viewPos;\n"
                               "uniform vec3 lightColor;\n"
                               "\n"
                               "layout(location = 0) out vec4 color;\n"
                               "layout(location = 1) out int fragmentID; // 输出到颜色附件的 ID\n"
                               "\n"
                               "void main()\n"
                               "{\n"
                               "    vec3 objectColor = vec3(VertexColor);\n"
                               "    vec3 norm = normalize(Normal);\n"
                               "    vec3 normNeg = -1 * norm;\n"
                               "\n"
                               "    // Ambient\n"
                               "    float ambientStrength = 0.1f;\n"
                               "    vec3 ambient = ambientStrength * lightColor;\n"
                               "\n"
                               "    // Diffuse\n"
                               "    vec3 lightDir = normalize(lightPos - FragPos);\n"
                               "    float diff = max(dot(norm, lightDir), 0.0);\n"
                               "    vec3 diffuse = diff * lightColor + max(dot(normNeg, lightDir), 0.0) * lightColor;\n"
                               "\n"
                               "    // Specular\n"
                               "    float specularStrength = 0.5f;\n"
                               "    vec3 viewDir = normalize(viewPos - FragPos);\n"
                               "    vec3 reflectDir = reflect(-lightDir, norm);\n"
                               "    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
                               "    vec3 reflectNegDir = reflect(-lightDir, normNeg);\n"
                               "    float specNeg = pow(max(dot(viewDir, reflectNegDir), 0.0), 32);\n"
                               "    vec3 specular = specularStrength * spec * lightColor + specularStrength * specNeg * lightColor;\n"
                               "\n"
                               "    vec3 result = (ambient + diffuse + specular) * objectColor;\n"
                               "    color = vec4(result, 1.0f);\n"
                               "\n"
                               "//    color = vec4(TriangleId / 206518.f, 0, 0, 1.0f);\n"
                               "\n"
                               "    fragmentID = TriangleId;\n"
                               "}";

    const GLchar *vShaderCode = vertexCode.c_str();
    const GLchar *fShaderCode = fragmentCode.c_str();
    // 2. 编译着色器
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];

    // 顶点着色器
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // 打印编译错误（如果有的话）
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // 片段着色器也类似
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // Print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 着色器程序
    this->Program = glCreateProgram();
    glAttachShader(this->Program, vertex);
    glAttachShader(this->Program, fragment);
    glLinkProgram(this->Program);
    // 打印连接错误（如果有的话）
    glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::Shader(const GLchar *vertexPath, const GLchar *fragmentPath) {
    // 1. 从文件路径中获取顶点/片段着色器
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // 保证ifstream对象可以抛出异常：
    vShaderFile.exceptions(std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::badbit);
    try {
        // 打开文件
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // 读取文件的缓冲内容到流中
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // 关闭文件
        vShaderFile.close();
        fShaderFile.close();
        // 转换流至GLchar数组
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const GLchar *vShaderCode = vertexCode.c_str();
    const GLchar *fShaderCode = fragmentCode.c_str();
    // 2. 编译着色器
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];

    // 顶点着色器
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // 打印编译错误（如果有的话）
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // 片段着色器也类似
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // Print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 着色器程序
    this->Program = glCreateProgram();
    glAttachShader(this->Program, vertex);
    glAttachShader(this->Program, fragment);
    glLinkProgram(this->Program);
    // 打印连接错误（如果有的话）
    glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::Use() {
    glUseProgram(this->Program);
}
