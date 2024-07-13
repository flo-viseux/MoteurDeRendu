#version 410

layout(location = 0) in vec3 in_position_os;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv_position;

uniform mat4 view_projection_matrix;
uniform mat4 transformation_matrix;
out vec2 uv;
out vec3 vertex_position;
out vec3 vNormal_ws;
out vec3 position_ws;

vec3 apply_matrix_to_position(mat4 matrix, vec3 point)
{
    vec4 tmp = matrix * vec4(point, 1.);
    return tmp.xyz / tmp.w;
}

vec3 apply_matrix_to_direction(mat4 matrix, vec3 direction)
{
    vec4 tmp = matrix * vec4(direction, 0.);
    return normalize(tmp.xyz);
}

void main()
{
    gl_Position = view_projection_matrix * transformation_matrix * vec4(in_position_os, 1.);
    uv = uv_position;
    vertex_position = gl_Position.xyz;
    position_ws = apply_matrix_to_position(view_projection_matrix, in_position_os);

    mat3 normalMatrix = transpose(inverse(mat3(transformation_matrix)));
    vNormal_ws = normalMatrix * normal;
}