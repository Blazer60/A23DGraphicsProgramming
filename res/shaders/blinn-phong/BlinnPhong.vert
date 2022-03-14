#version 460 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_uvs;
layout (location = 2) in vec3 a_normal;

uniform mat4 u_mvp;
uniform mat4 u_model_matrix;

out vec3 v_position_ws;
out vec2 v_uvs;
out vec3 v_normal;

void main()
{
    gl_Position = u_mvp * vec4(a_position.xyz, 1.f);
    v_uvs = a_uvs;
    v_normal = mat3(u_model_matrix) * a_normal;
    v_position_ws = vec3(u_model_matrix * vec4(a_position.xyz, 1.f));
}