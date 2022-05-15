#version 460 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_uvs;

uniform mat4 u_mvp_matrix;

out vec2 v_uvs;

void main()
{
    gl_Position = vec4(a_position, 1.f);
//    v_uvs = (position.xy + vec2(1.f));  // Works
    v_uvs = -a_position.xy;
}
