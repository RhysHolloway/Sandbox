#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec2 uv;

uniform mat4 camera;
uniform mat4 model;

void main() {
    uv = texCoords;
    gl_Position = camera * model * vec4(position, 1.0f);
}
