#version 460 core

in vec2 v_uvs;

uniform sampler2D u_diffuse;
uniform sampler2D u_specular;
uniform sampler2D u_albedo;

out layout(location = 0) vec3 o_colour;

void main()
{
    const vec3 diffuse  = texture(u_diffuse, v_uvs).rgb;
    const vec3 specular = texture(u_specular, v_uvs).rgb;
    const vec3 albedo   = texture(u_albedo, v_uvs).rgb;
    const vec3 ambient = 0.2f * albedo;

    o_colour = ambient + diffuse + specular;
}