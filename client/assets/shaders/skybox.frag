#version 330 core

out vec4 color;
in vec3 uv;
uniform samplerCube skybox;

void main()
{
   color = texture(skybox, vec3(uv.x, uv.y, -uv.z));
}