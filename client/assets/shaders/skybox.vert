#version 330 core

layout (location = 0) in vec3 position;
out vec3 texCoords;

uniform mat4 camera;

void main()
{
    texCoords = position;
    gl_Position = (camera * vec4(position, 1.0f)).xyww;
}