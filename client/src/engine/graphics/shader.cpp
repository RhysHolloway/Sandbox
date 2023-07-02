//
// Created by Rhys on 6/16/2023.
//

#include <fstream>
#include <format>
#include <GL/glew.h>

#include "../load.h"
#include "shader.h"

void Engine::Shader::from_source(const std::string &vertexSource, const std::string &fragmentSource) {
    const char *vShaderCode = vertexSource.c_str();
    const char *fShaderCode = fragmentSource.c_str();
    // 2. compile shaders
    GLuint vertex, fragment;
    int success;
    char infoLog[512];

    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        throw std::runtime_error(std::format("Could not compile vertex shader with error {}", success));
//        std::cout << "shader compile error" << std::endl;
    }

    // similiar for Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        throw std::runtime_error(std::format("Could not compile fragment shader with error {}", success));
    };

    // shader Program
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    // print linking errors if any
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        throw std::runtime_error(std::format("Could not link shader with id {}", id));
    }

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Engine::Shader::from_files(const std::string &vertexPath, const std::string &fragmentPath) {
    from_source(util::read_to_string(vertexPath), util::read_to_string(fragmentPath));
}

void Engine::Shader::use() const {
    glUseProgram(id);
};