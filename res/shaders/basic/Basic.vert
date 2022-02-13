#version 460 core

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec4 a_colour;

out vec4 v_colour;

void main()
{
    gl_Position = v_position;
    v_colour = a_colour;
}