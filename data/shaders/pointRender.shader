<<<VS
	#version 330

	uniform mat4 projview;
	uniform mat4 model;
	uniform vec3 scale;
	uniform float scalarRangeMax;
	uniform float scalarRangeMin;

	layout(location=0) in vec3 position;
	layout(location=1) in float radius;
	layout(location=2) in float scalar;

	out float scalar_out;

	void main()
	{
		scalar_out = scalar;

		gl_Position = projview * model * vec4(position, 1);
		if (!(scalar > scalarRangeMin && scalar < scalarRangeMax) && scalar < 1000)
		{
			gl_Position = projview * model * vec4(0,0,0,1);
		}
		gl_PointSize = radius * 2.0f;
		
	}
VS>>>

<<<FS
	#version 330

	out vec4 fragment;
	in float scalar_out;

	uniform vec3 color;

	void main()
	{
		//vec3 out_color = vec3(0.0f, 1.0f, 1.0f);
		//float s = scalar_out;
		//if (s < 0.0f)
		//{
		//	s = -s;
		//	float upperBound = 1.0f / 0.038f;
		//	out_color.g -= s * upperBound;
		//	
		//}
		//else
		//{
		//	float upperBound = 1.0f / 0.092f;
		//	out_color.b -= s * upperBound;
		//}

		//if(s > 0.0f && s < 0.025f)
		//fragment = vec4(out_color, 1);
		fragment = vec4(color, 1);
	}
FS>>>