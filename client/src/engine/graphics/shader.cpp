//
// Created by Rhys on 6/16/2023.
//

#include <fstream>
#include <format>
#include <GL/glew.h>

#include "../load.h"
#include "sandbox/util.h"
#include "shader.h"

Engine::Shader::Shader(const std::string &vertexSource, const std::string &fragmentSource, const char *name) {
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
    auto id = glCreateProgram();
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

    LOGGER << "Created shader #" << id << ", \"" << name << "\"" << std::endl;

    ID = id;
//    inner = std::make_shared<Engine::Shader::ShaderId>(ShaderId{id});
}

void Engine::Shader::use() const {
    glUseProgram(id());
}

unsigned int Engine::Shader::id() const noexcept {
    return ID;
}

int Engine::Shader::uniform_location(const std::string &name) {
    return glGetUniformLocation(id(), name.c_str());
}

//Engine::Shader::ShaderId::~ShaderId() {
//    LOGGER << "Deleted shader #" << id << std::endl;
//    glDeleteProgram(id);
//}
