#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 aUV;
//layout (location = 2) in vec3 normal;

out vec2 uv;

uniform mat4 camera;
uniform ivec3 relativeChunkPos;

void main() {
    uv = aUV;
    vec3 chunk = relativeChunkPos * ivec3(16);
    gl_Position = camera * vec4(chunk + position, 1.0f);
}
