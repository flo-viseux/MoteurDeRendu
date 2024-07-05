#version 410

out vec4 out_color;
uniform sampler2D my_texture;
in vec2 uv;

void main()
{
    vec4 texture_color = texture(my_texture, uv);

    //float4 color = tex.Sample(samp, input.Tex);
    float grey = dot(texture_color.rgb, vec3(.5, .5, .5));
    vec4 mask = vec4(1, 0, 0, 1);

    vec4 finalColor = texture_color * mask * vec4(vec3(grey), texture_color.a);

    out_color = finalColor;
}