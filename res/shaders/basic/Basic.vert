#version 460 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_colour;

uniform mat4 u_mvp;

out vec4 v_colour;

void main()
{
    gl_Position = u_mvp * vec4(a_position.xyz, 1.f);
    v_colour = vec4(a_colour, 1.0);
}