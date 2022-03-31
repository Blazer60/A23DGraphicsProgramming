#version 460 core

in vec2 v_uvs;



uniform sampler2D u_positions;
uniform sampler2D u_normals;
uniform sampler2D u_albedo;

out layout(location = 0) vec3 o_diffuse;
out layout(location = 1) vec3 o_specular;

void main()
{
    o_diffuse = vec3(1.f, 0.f, 0.f);
}
