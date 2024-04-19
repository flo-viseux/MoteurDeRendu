#version 410

layout(location = 0) in vec2 in_position;

float xOffset = .5f;
float yOffset = .5f; 
vec2 pos = vec2(in_position + vec2(xOffset, yOffset));

uniform float aspect_ratio;

void main()
{
    gl_Position = vec4(pos.x / aspect_ratio, pos.y, 0., 1.);
}