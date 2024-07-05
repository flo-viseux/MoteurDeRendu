#version 410

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv_position;

uniform mat4 view_projection_matrix;
uniform mat4 transformation_matrix;
out vec2 uv;
out vec3 vertex_position;
out vec3 vNormal_vs;
out vec3 fragmentPosition;

vec3 apply_matrix_to_position(mat4 matrix, vec3 point)
{
    vec4 tmp = matrix * vec4(point, 1.);
    return tmp.xyz / tmp.w;
}

void main()
{
    gl_Position = view_projection_matrix * vec4(in_position, 1.);
    uv = uv_position;
    vertex_position = gl_Position.xyz;
    fragmentPosition = apply_matrix_to_position(transformation_matrix, in_position);
    vNormal_vs = normal;
}