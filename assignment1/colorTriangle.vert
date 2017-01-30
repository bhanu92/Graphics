#version 330 core

layout (location = 1) in vec4 vPosition2;
layout (location = 2) in vec4 vColor;

out vec4 fs_color;

void
main()
{
    fs_color = vColor;
    gl_Position = vPosition2;
}
