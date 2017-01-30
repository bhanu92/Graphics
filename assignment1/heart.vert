#version 330 core

layout (location = 4) in vec4 vPosition4;

uniform vec4 inColor;
out vec4 outColor;

void main(){
        outColor = inColor;
        gl_Position = vPosition4;
}
