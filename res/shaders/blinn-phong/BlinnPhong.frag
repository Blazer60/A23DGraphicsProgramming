#version 460 core

in vec2 v_uvs;
in vec3 v_normal;
in vec3 v_positionWS;

uniform vec3 u_colour;
uniform sampler2D u_texture;
uniform vec3 u_lightDir;
uniform vec3 u_lightColour;
uniform mat4 u_viewMat;
uniform vec3 u_cameraPosWS;

out vec3 o_colour;

vec3 getCameraDirection()
{
    return normalize(u_cameraPosWS - v_positionWS);
}

vec3 halfAngle()
{
    return normalize(getCameraDirection() + u_lightDir);
}

float lightDot(vec3 a, vec3 b)
{
    return max(dot(a, b), 0.f);
}

void main()
{
    const float ambientIntensity  = 1.f * 0.1f;
    const vec3  diffuseIntensity  = 1.f * lightDot(v_normal, u_lightDir) * u_lightColour;
    const vec3  specularIntensity = 1.f * pow(lightDot(halfAngle(), v_normal), 128) * u_lightColour;

    o_colour = ambientIntensity * u_colour + diffuseIntensity * u_colour + specularIntensity * u_colour;
    o_colour = clamp(o_colour, 0.f, 1.f);  // We clamp here since opengl will overflow the number. HDR is not enabled.
}
