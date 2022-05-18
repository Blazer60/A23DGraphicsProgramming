#version 460 core

in      vec2        v_uvs;
in      vec3        v_normal_ws;
in      vec3        v_position_ws;

uniform vec3        u_colour;
uniform sampler2D   u_diffuse_texture;
uniform sampler2D   u_emissive_texture;

layout(location = 0) out vec3 o_position_ws;
layout(location = 1) out vec3 o_normal_ws;
layout(location = 2) out vec3 o_albedo;
layout(location = 3) out vec3 o_emissive;

void main()
{
    vec3 diffuse_map = texture(u_diffuse_texture, v_uvs).xyz;
    if (diffuse_map == vec3(0.0))
        diffuse_map = vec3(1.0);

    o_position_ws   = v_position_ws;
    o_normal_ws     = v_normal_ws;
    o_albedo        = u_colour * diffuse_map;
    o_emissive      = texture(u_emissive_texture, v_uvs).rgb;
}
