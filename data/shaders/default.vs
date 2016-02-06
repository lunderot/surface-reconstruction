#version 330

uniform mat4 projview;
uniform mat4 model;

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 uv;

out vec3 normal_out;
out vec2 uv_out;

void main()
{
	normal_out = normal;
	uv_out = uv;
	gl_Position = projview * model * vec4(position, 1);
}
