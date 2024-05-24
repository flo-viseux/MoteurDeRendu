#version 410

layout(location = 1) in vec2 uv_position;
layout(location = 2) in vec2 in_position;

out vec2 uv;

void main()
{
    gl_Position = vec4(in_position, 0., 1.);
    uv = uv_position;
}