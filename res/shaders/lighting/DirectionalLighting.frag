#version 460 core

in vec2 v_uvs;

uniform vec3 u_light_direction;
uniform vec3 u_light_intensity;

uniform vec3 u_camera_position_ws;

uniform sampler2D u_positions;
uniform sampler2D u_normals;
uniform sampler2D u_albedo;

out layout(location = 0) vec3 o_diffuse;
out layout(location = 1) vec3 o_specular;

vec3 get_camera_direction()
{
    const vec3 position = texture(u_positions, v_uvs).rgb;
    return normalize(u_camera_position_ws - position);
}

vec3 half_angle()
{
    return normalize(get_camera_direction() + u_light_direction);
}

float light_dot(vec3 a, vec3 b)
{
    return max(dot(a, b), 0.f);
}

void main()
{
    const vec3 normal   = texture(u_normals, v_uvs).rgb;
    const vec3 albedo   = texture(u_albedo, v_uvs).rgb;

    o_diffuse  = 1.f * albedo * light_dot(normal, u_light_direction)        * u_light_intensity;
    o_specular = 1.f * albedo * pow(light_dot(half_angle(), normal), 128.f) * u_light_intensity;
}