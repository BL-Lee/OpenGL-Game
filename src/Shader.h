#pragma once
#include <iostream>
#include <unordered_map>
#include "glm/gtc/matrix_transform.hpp"
struct ShaderSources
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	std::string m_filepath;
	uint32_t m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filepath);
	Shader();
	~Shader();

	void Load(const std::string& filepath);


	void Bind() const;
	void Unbind() const;

	//Set uniforms
		//set value or set uniform function with templates
	void SetUniform1i(const std::string& name, int i);
	void SetUniform1f(const std::string& name, float i);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

	void SetUniform1iv(const std::string& name, int count, const int& value);

	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

	uint32_t getRendererID() { return m_RendererID; }

private:
	ShaderSources ParseShader(const std::string& filepath);
	uint32_t CompileShader(const std::string& source, uint32_t type);
	uint32_t CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	uint32_t GetUniformLocation(const std::string& name);
};
