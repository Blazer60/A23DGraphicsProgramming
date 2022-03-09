#version 460 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_uvs;
layout (location = 2) in vec3 a_normal;

uniform mat4 u_mvp;
uniform mat4 u_modelMat;

out vec3 v_positionWS;
out vec2 v_uvs;
out vec3 v_normal;

void main()
{
    gl_Position = u_mvp * vec4(a_position.xyz, 1.f);
    v_uvs = a_uvs;
    v_normal = a_normal;
    v_positionWS = vec3(u_modelMat * vec4(a_position.xyz, 1.f));
}