#version 400 core

out vec4 color;
uniform vec4 icolor;

void main(void)
{
    color = icolor;
}
