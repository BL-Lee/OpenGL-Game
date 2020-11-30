#include "Shader.h"
#include <GL/glew.h>

#include "GLErrorCalls.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

/**
* Sets up shaders
*/




Shader::Shader(const std::string& filepath)
	: m_filepath(filepath), m_RendererID(0)
{
    //Parses the shader and then creates it with a unique id
    ShaderSources source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}
void Shader::Load(const std::string& filepath)
    
{
    m_filepath = filepath;
    m_RendererID = 0;
    //Parses the shader and then creates it with a unique id
    ShaderSources source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}
Shader::~Shader() 
{
    glDeleteProgram(m_RendererID);
}

uint32_t Shader::CompileShader(const std::string& source, uint32_t type)
{
    
    uint32_t id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    //Debugging
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

/**
* Method to parse the file into vertex and fragment shaders
*/
ShaderSources Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    { //will need to add more if further shader types are needed
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;

    //buffer to hold the shaders
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        //line.find returns a position in the string
        //and npos is an invalid string location
        //So this runs when #shader is in the line
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        //Otherwise it is any other line
        else
        {
            //uses type as an index for ss
            ss[(int)type] << line << "\n";
        }
    }
    return { ss[0].str(),ss[1].str() };
}


uint32_t Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    uint32_t program = glCreateProgram();
    uint32_t vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
    uint32_t fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDetachShader(program, vs);
    glDetachShader(program, fs);

    return program;
}

void Shader::Bind() const
{
    glUseProgram(m_RendererID);
}
void Shader::Unbind() const
{
    glDeleteProgram(m_RendererID);
}

//Set uniforms
//May want to later set up a template with uniforms, or a switch or something for different uniform types
void Shader::SetUniform1i(const std::string& name, int i)
{
    glUniform1i(GetUniformLocation(name), i);
}

void Shader::SetUniform1f(const std::string& name, float f)
{
    glUniform1f(GetUniformLocation(name), f);
}
void Shader::SetUniform4f(const std::string& name,
	float v0, float v1, float v2, float v3)
{
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}
void Shader::SetUniform1iv(const std::string& name,  int count, const int& value)
{
    glUniform1iv(GetUniformLocation(name), count, &value);
}
void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}




uint32_t Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    int location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1)
    {
        std::cout << "Warning: uniform (" << name << ") not found in shader" << std::endl;
    }
    else
        m_UniformLocationCache[name] = location;
    return location;
}

