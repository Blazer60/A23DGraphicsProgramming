#version 460

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_uvs;

uniform mat4 u_mvp;
uniform mat4 u_model_matrix;
uniform vec3 u_halfsize;

out vec3 v_position_ws;

void main()
{
    gl_Position = u_mvp * vec4(u_halfsize * a_position, 1.f);
    v_position_ws = (u_model_matrix * vec4(u_halfsize * a_position, 1.f)).xyz;
}
