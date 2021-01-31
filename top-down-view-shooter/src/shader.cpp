#include "Shader.h"

#include <vector>
#include <fstream>

#include "glad/gl.h"

namespace
{
	std::string LoadShader(std::string_view filename)
	{
		FILE * f = NULL;
		errno_t err = fopen_s(&f, filename.data(), "rb");
		if (err)
		{
			fprintf(stderr, "failed to load shader from %s: error code %d", filename.data(), err);
			return {};
		}

		fseek(f, 0, SEEK_END);
		long length = ftell(f);
		fseek(f, 0, SEEK_SET);

		std::string shader(length, '\0');
		size_t read = fread(shader.data(), 1, length, f);

		fclose(f);

		if (read != length)
		{
			fprintf(stderr, "failed to read data from %s", filename.data());
			return {};
		}
		return shader;
	}

	bool CheckShader(GLint shader, const char * desc)
	{
		GLint status = 0;		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		GLint logLength = 0;	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		if (static_cast<GLboolean>(status) == GL_FALSE)
		{
			fprintf(stderr, "failed to compile: %s!\n", desc);
		}
		if (logLength > 0)
		{
			std::vector<char> log(logLength + 1);
			glGetShaderInfoLog(shader, logLength, NULL, log.data());
			fprintf(stderr, "%s\n", log.data());
		}
		return static_cast<GLboolean>(status) == GL_TRUE;
	}

	bool CheckProgram(GLint program, const char * desc)
	{
		GLint status = 0;		glGetProgramiv(program, GL_LINK_STATUS, &status);
		GLint logLength = 0;	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		if (static_cast<GLboolean>(status) == GL_FALSE)
		{
			fprintf(stderr, "failed to link: %s!\n", desc);
		}
		if (logLength > 0)
		{
			std::vector<char> log(logLength + 1);
			glGetProgramInfoLog(program, logLength, NULL, log.data());
			fprintf(stderr, "%s\n", log.data());
		}
		return static_cast<GLboolean>(status) == GL_TRUE;
	}
}


Shader Shader::Load(std::string_view vertexShaderFile, std::string_view fragmentShaderFile)
{
	std::string vertexShader = LoadShader(vertexShaderFile);
	std::string fragmentShader = LoadShader(fragmentShaderFile);
	return Shader(vertexShader, fragmentShader);
}

Shader::Shader(Shader&& other) noexcept
	: m_shaderProgram(other.m_shaderProgram)
{
	other.m_shaderProgram = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept
{
	if (&other != this)
	{
		Shader temp(std::move(other));
		swap(*this, temp);
	}
	return *this;
}

void swap(Shader& lhs, Shader& rhs) noexcept
{
	std::swap(lhs.m_shaderProgram, rhs.m_shaderProgram);
}

Shader::Shader(std::string_view vertexShaderSource, std::string_view fragmentShaderSource)
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar * data = vertexShaderSource.data();
	GLint length = static_cast<GLint>(vertexShaderSource.length());
	glShaderSource(vertexShader, 1, &data, &length);
	glCompileShader(vertexShader);
	bool compiled = CheckShader(vertexShader, "vertex shader");

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	data = fragmentShaderSource.data();
	length = static_cast<GLint>(fragmentShaderSource.length());
	glShaderSource(fragmentShader, 1, &data, &length);
	glCompileShader(fragmentShader);
	compiled &= CheckShader(fragmentShader, "fragment shader");

	if (compiled)
	{
		GLuint shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		bool linked = CheckProgram(shaderProgram, "shader program");

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		if (linked)
		{
			m_shaderProgram = shaderProgram;
		}
		else
		{
			throw std::runtime_error("Linker error");
		}
	}
	else
	{
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		throw std::runtime_error("Compiler error");
	}
}

Shader::~Shader()
{
	if (m_shaderProgram)
	{
		glDeleteProgram(m_shaderProgram);
	}
}

void Shader::Use()
{
	glUseProgram(m_shaderProgram);
}

void Shader::SetBool(std::string_view var, bool value)
{
	GLint location = glGetUniformLocation(m_shaderProgram, var.data());
	glUniform1i(location, static_cast<GLuint>(value));
}

void Shader::SetFloat(std::string_view var, float value)
{
	GLint location = glGetUniformLocation(m_shaderProgram, var.data());
	glUniform1f(location, value);
}

void Shader::SetFloat(std::string_view var, float v1, float v2)
{
	GLint location = glGetUniformLocation(m_shaderProgram, var.data());
	glUniform2f(location, v1, v2);
}

void Shader::SetFloat(std::string_view var, float v1, float v2, float v3)
{
	GLint location = glGetUniformLocation(m_shaderProgram, var.data());
	glUniform3f(location, v1, v2, v3);
}

void Shader::SetInt(std::string_view var, int value)
{
	GLint location = glGetUniformLocation(m_shaderProgram, var.data());
	glUniform1i(location, value);
}
