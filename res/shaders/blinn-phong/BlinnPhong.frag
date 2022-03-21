#version 460 core

in      vec2        v_uvs;
in      vec3        v_normal;
in      vec3        v_position_ws;

uniform vec3        u_colour;
uniform sampler2D   u_texture;
uniform vec3        u_light_direction;
uniform vec3        u_light_colour;
uniform mat4        u_view_matrix;
uniform vec3        u_camera_position_ws;

layout(location = 0) out vec3 o_colour;

vec3 get_camera_direction()
{
    return normalize(u_camera_position_ws - v_position_ws);
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
    vec3 diffuse_map = texture(u_texture, v_uvs).xyz;
    if (diffuse_map == vec3(0.0))
        diffuse_map = vec3(1.0);

    const vec3  ambient_intensity  = 1.f * 0.1f * u_light_colour;
    const vec3  diffuse_intensity  = 1.f * light_dot(v_normal, u_light_direction) * u_light_colour;
    const vec3  specular_intensity = 1.f * pow(light_dot(half_angle(), v_normal), 128) * u_light_colour;

    o_colour    = ambient_intensity  * u_colour * diffuse_map
                + diffuse_intensity  * u_colour * diffuse_map
                + specular_intensity * u_colour * diffuse_map;

    o_colour = clamp(o_colour, 0.f, 1.f);  // We clamp here since opengl will overflow the number. HDR is not enabled.
}
