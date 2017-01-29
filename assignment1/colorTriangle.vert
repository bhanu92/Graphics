#version 330 core

layout (location = 1 ) in vec4 vPosition2
layout (location = 3 ) in vec4 bufferColor

out vec4 color;

void main()
{
        color = bufferColor;
        gl_Position = vPosition2;
}
