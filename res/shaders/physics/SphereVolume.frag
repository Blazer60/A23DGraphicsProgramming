#version 460

in vec3 v_position_ws;

uniform vec3 u_camera_position_ws;

layout (location = 0) out vec3 o_position_ws;
layout (location = 1) out vec3 o_normal_ws;
layout (location = 2) out vec3 o_albedo;

void main()
{
    o_position_ws = v_position_ws;
    o_normal_ws = normalize(u_camera_position_ws - v_position_ws);
    o_albedo = vec3(1.f);
}
