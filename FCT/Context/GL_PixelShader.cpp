#include "../headers.h"
#include <iostream>

namespace FCT {

GL_PixelShader::GL_PixelShader(const ShaderOutput& vertexOutput)
    : PixelShader(vertexOutput), m_shaderID(0)
{
}

GL_PixelShader::~GL_PixelShader()
{
    if (m_shaderID != 0) {
        glDeleteShader(m_shaderID);
    }
}

bool GL_PixelShader::compile()
{
    // Delete any existing shader
    if (m_shaderID != 0) {
        glDeleteShader(m_shaderID);
    }

    // Create a new shader
    m_shaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Set the shader source
    const char* source = m_source.c_str();
    glShaderSource(m_shaderID, 1, &source, NULL);

    // Compile the shader
    glCompileShader(m_shaderID);

    // Check for compilation errors
    GLint success;
    glGetShaderiv(m_shaderID, GL_COMPILE_STATUS, &success);

    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(m_shaderID, 512, NULL, infoLog);
        m_compileError = "Shader compilation failed: " + std::string(infoLog);
        m_isCompiled = false;
        return false;
    }

    m_isCompiled = true;
    return true;
}

} // namespace FCT