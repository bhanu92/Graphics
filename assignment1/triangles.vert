#version 330 core

layout( location = 0 ) in vec4 vPosition1;

uniform vec4 inColor;
out vec4 outColor;

void
main()
{
        outColor = inColor;
        gl_Position = vPosition1;
}
