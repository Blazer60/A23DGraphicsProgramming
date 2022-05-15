#version 460

in vec2 v_uvs;

uniform vec3 u_light_key_threshold;
uniform sampler2D u_texture;

out layout(location = 0) vec3 o_output;

void main()
{
    o_output = vec3(texture(u_texture, v_uvs)) - u_light_key_threshold;
}
