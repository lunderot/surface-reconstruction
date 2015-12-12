#version 150

uniform mat4 projview;
uniform mat4 model;

in vec3 position;

void main()
{
	gl_Position = projview * model * vec4(position, 1);
}