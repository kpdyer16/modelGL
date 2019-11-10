#version 400 core

in vec4 vPosition;
uniform mat4x4 pers_cam_xform;


void main(void)
{
    gl_Position = pers_cam_xform * vPosition;
}
