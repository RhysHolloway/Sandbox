#version 330 core

layout(location = 0) vec3 coord;
layout(location = 1) vec2 texCoordsIn;

out vec2 texCoords;

void main() {
    gl_position = vec4(coord, 1.0f);
    texCoords = texCoordsIn;
}