#version 330

out vec4 fragment;

in vec2 uv_out;
in vec3 normal_out;

void main()
{
    fragment = vec4(uv_out, 1.0, 1.0);
}
