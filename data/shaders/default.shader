<<<VS
	#version 330

	uniform mat4 projview;
	uniform mat4 viewmodel;
	uniform mat4 model;
	uniform mat3 normal_mat;
	uniform vec3 scale;
	uniform bool scaleuv;

	layout(location=0) in vec3 position_vs;
	layout(location=1) in vec3 normal_vs;
	layout(location=2) in vec2 uv_vs;

	out vec3 position_fs;
	out vec3 normal_fs;
	out vec2 uv_fs;

	void main()
	{
		if(scaleuv)
		{
			uv_fs.x = uv_vs.x * max(scale.x, scale.z);
			uv_fs.y = uv_vs.y * scale.y;
		}
		else
		{
			uv_fs = uv_vs;
		}
		normal_fs = normal_mat * normal_vs;
		position_fs = vec3(viewmodel * vec4(position_vs, 1));
		gl_Position = projview * model * vec4(position_vs, 1);
	}
VS>>>

<<<FS
	#version 330

	uniform bool textured;
	uniform sampler2D tex;
	uniform vec3 color;

	in vec3 position_fs;
	in vec3 normal_fs;
	in vec2 uv_fs;

	out vec4 fragment;

	const vec3 light_position = vec3(0.0, 0.0, -2.0);
	const vec3 ambient_color = vec3(0.1, 0.1, 0.1);
	const vec3 diffuse_color = vec3(0.5, 0.5, 0.5);
	const vec3 specular_color = vec3(1.0, 1.0, 1.0);
	const float shininess = 1000.0;

	void main()
	{
		vec3 normal = normalize(normal_fs);
		vec3 light_direction = normalize(light_position - position_fs);

		float lambertian = max(dot(light_direction, normal), 0.0);
		//float specular = 0.0;

		//if (lambertian > 0.0) {
		//	vec3 view_direction = normalize(-position_fs);
		//	vec3 half_direction = normalize(light_direction + view_direction);
		//	float spec_angle = max(dot(half_direction, normal), 0.0);
		//	specular = pow(spec_angle, shininess);
		//} 

		vec3 color_linear = ambient_color + lambertian * diffuse_color; //+specular * specular_color;

		if (textured)
		{
			fragment = texture(tex, uv_fs);
		}
		else
		{
			fragment = vec4(color, 1.0f);
		}
		
		fragment += vec4(color_linear, 1.0f);
	}
FS>>>