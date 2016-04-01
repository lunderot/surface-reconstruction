<<<VS
	#version 330

	uniform mat4 projview;
	uniform mat4 model;
	uniform vec3 scale;

	layout(location=0) in vec3 position;
	layout(location=1) in float scale;

	void main()
	{
		gl_Position = projview * model * vec4(position, 1);
	}
VS>>>

<<<FS
	#version 330

	out vec4 fragment;

	void main()
	{
		fragment = vec4(1, 0, 0, 1);
	}
FS>>>