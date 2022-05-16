#version 460

in vec2 v_uvs;

uniform sampler2D u_original;
uniform sampler2D u_bloom;

out layout(location = 0) vec3 o_output;

void main()
{
    const float gamma = 2.2f;
    const float exposure = 1.f;
    const vec3 original_colour = max(texture(u_original, v_uvs).rgb, vec3(0.f));
    const vec3 bloom_colour = max(textureLod(u_bloom, v_uvs, 1.f).rgb, vec3(0.f));

    vec3 result = vec3(1.f) - exp(-(original_colour + bloom_colour) * exposure);
    o_output = result;
}
