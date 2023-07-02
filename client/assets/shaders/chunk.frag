#version 330 core

in vec2 uv;
in vec3 normal;
in vec3 light;

out vec4 color;

uniform sampler2D atlas;

void main() {
    color = vec4(light, 1.0) * texture(atlas, uv);
}