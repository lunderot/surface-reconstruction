#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>

class Shader
{
private:
	static const unsigned int NUM_SHADERS = 2;
	GLuint program;
	GLuint shader[NUM_SHADERS];
public:
	Shader(const std::string& filename);
	~Shader();

	bool IsInUse() const;
	void Use() const;

	void SetUniform(const GLchar* name, const int& scalar);
	void SetUniform(const GLchar* name, const glm::float32& scalar);
	void SetUniform(const GLchar* name, const glm::vec2& vector);
	void SetUniform(const GLchar* name, const glm::vec3& vector);
	void SetUniform(const GLchar* name, const glm::vec4& vector);

	void SetUniform(const GLchar* name, const glm::mat4& matrix);
private:
	std::string LoadFile(const std::string& filename);
	GLuint CreateShader(const std::string& filename, GLenum shaderType);

	void CheckShaderError(GLuint shader, GLuint flag, const std::string& errorMessage);
	void CheckProgramError(GLuint program, GLuint flag, const std::string& errorMessage);
};

