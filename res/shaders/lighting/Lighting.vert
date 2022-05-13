#version 460 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_uvs;

uniform mat4 u_model_matrix;

out vec2 v_uvs;

void main()
{
//    v_uvs = vec2(u_model_matrix * vec4(a_uvs, 0.f, 1.f));
    vec4 position = u_model_matrix * vec4(a_position, 1.f);
    gl_Position = position;
    v_uvs = ((position.xy / position.w) + vec2(1.f)) / 2.f;
}
