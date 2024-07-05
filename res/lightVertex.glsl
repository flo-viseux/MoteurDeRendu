#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;


out vec2 v_uv;
//out vec3 v_normal;
out vec3 vNormal_vs;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

void main()
{
    gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(aPos, 1.0);
    v_uv = aTexCoord;
    vNormal_vs = mat3(transpose(inverse(uModelMatrix))) * aNormal;
}