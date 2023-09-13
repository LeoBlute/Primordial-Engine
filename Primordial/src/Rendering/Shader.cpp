#include "Glad/glad.h"
#include "glm/glm.hpp"
#include "VUtils/FileUtils.hpp"
#include "VUtils/DebugUtils.hpp"
#include "Shader.hpp"

Renderer2D::Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
    const char* vertexShaderCode = FileUtils::CPReadFile(vertexShaderPath);
    const char* fragmentShaderCode = FileUtils::CPReadFile(fragmentShaderPath);
    const unsigned int vertexShader = CreateShader(vertexShaderCode, GL_VERTEX_SHADER);
    const unsigned int fragmentShader = CreateShader(fragmentShaderCode, GL_FRAGMENT_SHADER);

    delete[] vertexShaderCode;
    delete[] fragmentShaderCode;

    mID = glCreateProgram();
    glAttachShader(mID, vertexShader);
    glAttachShader(mID, fragmentShader);
    glLinkProgram(mID);

    int sucess;
    char infoLog[512];
    glGetProgramiv(mID, GL_LINK_STATUS, &sucess);
    if (!sucess)
    {
        glGetProgramInfoLog(mID, 512, NULL, infoLog);
        DEBUG_ERROR("Program failed to link");
        DEBUG_ERROR(infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Renderer2D::Shader::~Shader()
{
    glDeleteProgram(mID);
}

void Renderer2D::Shader::Use()
{
    glUseProgram(mID);
}

void Renderer2D::Shader::SetBool(const char* name, bool value)
{
    glUniform1i(glGetUniformLocation(mID, name), (int)value);
}

void Renderer2D::Shader::SetVec2(const char* name, const glm::vec2& value)
{
    glUniform2fv(glGetUniformLocation(mID, name), 1, &value[0]);
}

void Renderer2D::Shader::SetVec3(const char* name, const glm::vec3& value)
{
    glUniform3fv(glGetUniformLocation(mID, name), 1, &value[0]);
}

void Renderer2D::Shader::SetVec4(const char* name, const glm::vec4& value)
{
    glUniform4fv(glGetUniformLocation(mID, name), 1, &value[0]);
}

void Renderer2D::Shader::SetMat4(const char* name, const glm::mat4& value)
{
    glUniformMatrix4fv(glGetUniformLocation(mID, name), 1, GL_FALSE, &value[0][0]);
}

const unsigned int Renderer2D::Shader::CreateShader(const char* code, int type)
{
    //Creates shader
    const unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &code, NULL);
    glCompileShader(shader);

    //Checks if shader could compile
    int sucess;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &sucess);
    if (!sucess)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << code << std::endl;
        DEBUG_WARN("Shader failed to compile");
        std::cout << type << std::endl;
        DEBUG_ERROR(infoLog);
    }

    return shader;
}