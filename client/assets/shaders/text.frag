#version 330 core

in vec2 texCoords;

uniform sampler2D atlas;
uniform vec4 color;

void main() {
    gl_FragColor = vec4(1.f, 1.f, 1.f, texture(atlas, texCoords).r) * color;
}