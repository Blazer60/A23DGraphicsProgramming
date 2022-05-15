#version 460

in vec2 v_uvs;

uniform int u_mip_level;
uniform sampler2D u_texture;
uniform vec2 u_mip_size;

out layout(location = 0) vec3 o_output;

void main()
{
    ivec2 i_uv = ivec2(v_uvs * u_mip_size);
    o_output = vec3(texelFetch(u_texture, i_uv, u_mip_level));
}
