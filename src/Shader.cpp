#include "Shader.h"

#include <iostream>
#include <SDL_assert.h>

Shader::Shader(const std::string& filename)
{
	program = glCreateProgram();

	shader[0] = CreateShader(filename + ".vs", GL_VERTEX_SHADER);
	shader[1] = CreateShader(filename + ".fs", GL_FRAGMENT_SHADER);

	for (int i = 0; i < NUM_SHADERS; i++)
	{
		glAttachShader(program, shader[i]);
	}

	glLinkProgram(program);
	CheckProgramError(program, GL_LINK_STATUS, "Failed to link program: " + filename);

	glValidateProgram(program);
	CheckProgramError(program, GL_VALIDATE_STATUS, "Program is not valid: " + filename);
}


Shader::~Shader()
{
	for (int i = 0; i < NUM_SHADERS; i++)
	{
		glDetachShader(program, shader[i]);
		glDeleteShader(shader[i]);
	}
	glDeleteProgram(program);
}

bool Shader::IsInUse() const
{
	GLint currentProgram = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

	return currentProgram == (GLint)program;
}

void Shader::Use() const
{
	glUseProgram(program);
}

void Shader::SetUniform(const GLchar* name, const glm::int32& scalar)
{
	SDL_assert(IsInUse() && "This shader does not match current shader on the GPU");
	glUniform1i(glGetUniformLocation(program, name), scalar);
}

void Shader::SetUniform(const GLchar* name, const glm::float32& scalar)
{
	SDL_assert(IsInUse() && "This shader does not match current shader on the GPU");
	glUniform1f(glGetUniformLocation(program, name), scalar);
}

void Shader::SetUniform(const GLchar* name, const glm::vec2& vector)
{
	SDL_assert(IsInUse() && "This shader does not match current shader on the GPU");
	glUniform2fv(glGetUniformLocation(program, name), 1, glm::value_ptr(vector));
}

void Shader::SetUniform(const GLchar* name, const glm::vec3& vector)
{
	SDL_assert(IsInUse() && "This shader does not match current shader on the GPU");
	glUniform3fv(glGetUniformLocation(program, name), 1, glm::value_ptr(vector));
}

void Shader::SetUniform(const GLchar* name, const glm::vec4& vector)
{
	SDL_assert(IsInUse() && "This shader does not match current shader on the GPU");
	glUniform4fv(glGetUniformLocation(program, name), 1, glm::value_ptr(vector));
}

void Shader::SetUniform(const GLchar* name, const glm::mat4& matrix)
{
	SDL_assert(IsInUse() && "This shader does not match current shader on the GPU");
	glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

std::string Shader::LoadFile(const std::string& filename)
{
	std::ifstream file(filename);
	std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
	file.close();
	return content;
}

GLuint Shader::CreateShader(const std::string& filename, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	std::string shaderText = LoadFile(filename);
	const GLchar* shaderSource[1];
	GLint shaderSourceLength[1];
	shaderSource[0] = shaderText.c_str();
	shaderSourceLength[0] = shaderText.length();

	glShaderSource(shader, 1, shaderSource, shaderSourceLength);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, "Failed to compile shader: " + filename);

	return shader;
}

void Shader::CheckShaderError(GLuint shader, GLuint flag, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		glGetShaderInfoLog(shader, sizeof(error), NULL, error);
		throw std::runtime_error(errorMessage + ": " + error);
	}
}

void Shader::CheckProgramError(GLuint program, GLuint flag, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	glGetProgramiv(program, flag, &success);

	if (success == GL_FALSE)
	{
		glGetProgramInfoLog(program, sizeof(error), NULL, error);
		throw std::runtime_error(errorMessage + ": " + error);
	}
}
