<<<VS
	#version 330

	uniform mat4 projview;
	uniform mat4 model;
	uniform vec3 scale;

	layout(location=0) in vec3 position;
	layout(location=1) in float radius;
	layout(location=2) in float scalar;

	out float scalar_out;

	void main()
	{
		scalar_out = scalar;
		gl_PointSize = radius * 2.0f;
		gl_Position = projview * model * vec4(position, 1);
	}
VS>>>

<<<FS
	#version 330

	out vec4 fragment;
	in float scalar_out;

	void main()
	{
		fragment = vec4(scalar_out, 0, 0, 1);
	}
FS>>>