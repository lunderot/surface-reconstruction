#version 330

out vec4 fragment;

in vec2 uv_out;
in vec3 normal_out;

uniform sampler2D tex;

void main()
{
    fragment = texture(tex, uv_out);
}
