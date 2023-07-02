#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 aUV;
layout (location = 2) in uint bit_params;

out vec2 uv;
out vec3 normal;
out vec3 light;

uniform mat4 camera;
uniform ivec3 relativeChunkPos;

vec3 get_normal(uint i) {
    switch (i) {
        case 0:
            return vec3(1.0, 0.0, 0.0);
        case 1:
            return vec3(-1.0, 0.0, 0.0);
        case 2:
            return vec3(0.0, 1.0, 0.0);
        case 3:
            return vec3(1.0, -1.0, 0.0);
        case 4:
            return vec3(0.0, 0.0, 1.0);
        case 5:
            return vec3(0.0, 0.0, -1.0);
        default:
            return vec3(1.0, 1.0, 1.0);
    }
}

void main() {
    uv = aUV;
    normal = get_normal(bit_params >> 12 & 8);
    light = vec3(float(bit_params & 0xF), float((bit_params >> 4) & 0xF), float((bit_params >> 8) & 0xF)) / 15.0;
    vec3 chunk = relativeChunkPos * ivec3(16, -16, 16);
    gl_Position = camera * vec4(chunk + position, 1.0f);
}
