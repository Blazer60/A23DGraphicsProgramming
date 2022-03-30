#version 460 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_uvs;

uniform mat4 u_model_matrix;

out vec2 v_uvs;

void main()
{
    v_uvs = a_uvs;
    gl_Position = u_model_matrix * vec4(a_position, 1.f);
}
