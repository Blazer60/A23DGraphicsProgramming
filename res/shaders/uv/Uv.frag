#version 460 core

in vec2 v_uvs;

uniform vec3 u_colour;
uniform sampler2D u_texture;

void main()
{
    gl_FragColor = vec4(u_colour, 1.0) * texture(u_texture, v_uvs);
}
