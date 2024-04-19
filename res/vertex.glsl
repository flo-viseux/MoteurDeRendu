#version 410

layout(location = 0) in vec2 in_position;

uniform float xOffset;
uniform float yOffset;
uniform float aspect_ratio;
uniform float time;

vec2 pos = vec2(in_position.x + xOffset, in_position.y + yOffset);

void main()
{
    gl_Position = vec4((pos.x / aspect_ratio) + cos(time) / 2, pos.y + sin(time) / 2, 0., 1.);
}