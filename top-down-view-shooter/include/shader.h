#pragma once

#include <string_view>

class Shader
{
public:
	static Shader Load(std::string_view vertexShaderFile, std::string_view fragmentShaderFile);

	Shader() = default;
	Shader(const Shader &) = delete;
	Shader& operator=(const Shader &) = delete;
	Shader(Shader && other) noexcept;
	Shader& operator=(Shader && other) noexcept;
	~Shader();
	
	friend void swap(Shader & lhs, Shader & rhs) noexcept;
	
	Shader(std::string_view vertexShaderSource, std::string_view fragmentShaderSource);

	void Use();
	
	void SetBool(std::string_view var, bool value);
	void SetInt(std::string_view var, int value);
	void SetFloat(std::string_view var, float value);
	void SetFloat(std::string_view var, float v1, float v2);
	void SetFloat(std::string_view var, float v1, float v2, float v3);
	
private:
	unsigned int m_shaderProgram = 0;
};