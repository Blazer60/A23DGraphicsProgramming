#version 460

in vec2 v_uvs;

uniform sampler2D u_texture;
uniform vec2 u_mip_size;
uniform int u_mip_level;

out layout(location = 0) vec3 o_output;

void main()
{
    ivec2 center = ivec2(v_uvs * u_mip_size);

    vec3 texel_a = texelFetch(u_texture, center + ivec2(-2, +2), u_mip_level).rgb;
    vec3 texel_b = texelFetch(u_texture, center + ivec2(+0, +2), u_mip_level).rgb;
    vec3 texel_c = texelFetch(u_texture, center + ivec2(+2, +2), u_mip_level).rgb;

    vec3 texel_d = texelFetch(u_texture, center + ivec2(-1, +1), u_mip_level).rgb;
    vec3 texel_e = texelFetch(u_texture, center + ivec2(+1, +1), u_mip_level).rgb;

    vec3 texel_f = texelFetch(u_texture, center + ivec2(-2, +0), u_mip_level).rgb;
    vec3 texel_g = texelFetch(u_texture, center + ivec2(-0, +0), u_mip_level).rgb;
    vec3 texel_h = texelFetch(u_texture, center + ivec2(+2, +0), u_mip_level).rgb;

    vec3 texel_i = texelFetch(u_texture, center + ivec2(-1, -1), u_mip_level).rgb;
    vec3 texel_j = texelFetch(u_texture, center + ivec2(+1, -1), u_mip_level).rgb;

    vec3 texel_k = texelFetch(u_texture, center + ivec2(-2, -2), u_mip_level).rgb;
    vec3 texel_l = texelFetch(u_texture, center + ivec2(+0, -2), u_mip_level).rgb;
    vec3 texel_m = texelFetch(u_texture, center + ivec2(+2, -2), u_mip_level).rgb;

    vec3 yellow_sector = 0.125f * (texel_a + texel_b + texel_f + texel_g) / 4.f;
    vec3 green_sector  = 0.125f * (texel_b + texel_c + texel_g + texel_h) / 4.f;
    vec3 red_sector    = 0.5f   * (texel_d + texel_e + texel_i + texel_j) / 4.f;
    vec3 purple_sector = 0.125f * (texel_f + texel_g + texel_k + texel_l) / 4.f;
    vec3 blue_sector   = 0.125f * (texel_g + texel_h + texel_l + texel_m) / 4.f;

    o_output = yellow_sector + green_sector + red_sector + purple_sector + blue_sector;
}
