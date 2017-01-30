#version 330 core

layout (location = 2) in vec4 vPosition3;

uniform vec4 inColor;
out vec4 outColor;

void main(){
        outColor = inColor;
        gl_Position = vPosition3;
}
