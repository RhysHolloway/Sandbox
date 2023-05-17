//
// Created by Rhys on 5/16/2023.
//
#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

std::string open_file(const char * path) {
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    file.open(path);
    std::stringstream stream;
    stream << file.rdbuf();
    return stream.str();
}

class Shader {

public:

    GLuint id;

    Shader() = default;

    Shader(const char *vertexPath, const char *fragmentPath) {
        string vertexCode, fragmentCode;
        try {
            vertexCode = open_file(vertexPath);
            fragmentCode = open_file(fragmentPath);
        }
        catch (std::ifstream::failure e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char *vShaderCode = vertexCode.c_str();
        const char *fShaderCode = fragmentCode.c_str();
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
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        };

// similiar for Fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
// print compile errors if any
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
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
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

// delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void use()
    {
        glUseProgram(id);
    }
};
