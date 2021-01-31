#pragma once

#include <string_view>

class Shader
{
public:
	static Shader Load(std::string_view vertexShaderFile, std::string_view fragmentShaderFile);
	
	Shader(std::string_view vertexShaderSource, std::string_view fragmentShaderSource);

	void Use();
	
	void SetBool(std::string_view var, bool value);
	void SetFloat(std::string_view var, float value);
	void SetInt(std::string_view var, int value);
	
private:
	unsigned int m_shaderProgram = 0;
};