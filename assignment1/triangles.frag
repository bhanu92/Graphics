#version 330 core

in vec4 outColor;
out vec4 fs_Color;

void main()
{
    fs_Color = outColor;
}
