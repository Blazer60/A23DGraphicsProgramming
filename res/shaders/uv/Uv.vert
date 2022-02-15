#version 460 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_uvs;

uniform mat4 u_mvp;

void main()
{
    gl_Position = u_mvp * vec4(a_position.xyz, 1.f);
}