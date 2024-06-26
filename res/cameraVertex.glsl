#version 410

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 uv_position;

uniform mat4 view_projection_matrix;
out vec2 uv;
out vec3 vertex_position;

void main()
{
    gl_Position = view_projection_matrix * vec4(in_position, 1.);
    uv = uv_position;
    vertex_position = in_position;
}