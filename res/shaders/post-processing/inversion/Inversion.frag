#version 460 core

in vec2 v_uvs;

uniform sampler2D u_texture;

void main()
{
    vec4 texColour = texture(u_texture, v_uvs);

    gl_FragColor = vec4(vec3(1.0) - texColour.rgb, 1.0);
}
