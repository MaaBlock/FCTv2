#include "../headers.h"
#include <iostream>

namespace FCT {

    GL_VertexShader::GL_VertexShader(VertexFactory* factory)
        : VertexShader(factory), m_shaderId(0)
    {
    }

    GL_VertexShader::~GL_VertexShader()
    {
        if (m_shaderId != 0) {
            glDeleteShader(m_shaderId);
        }
    }

    bool GL_VertexShader::compile()
    {
        if (m_shaderId != 0) {
            glDeleteShader(m_shaderId);
        }

        m_shaderId = glCreateShader(GL_VERTEX_SHADER);
        if (m_shaderId == 0) {
            m_compileError = "Failed to create shader object";
            m_isCompiled = false;
            return false;
        }

        const char* sourcePtr = m_source.c_str();
        glShaderSource(m_shaderId, 1, &sourcePtr, nullptr);
        glCompileShader(m_shaderId);

        GLint compileStatus;
        glGetShaderiv(m_shaderId, GL_COMPILE_STATUS, &compileStatus);

        if (compileStatus == GL_FALSE) {
            GLint infoLogLength;
            glGetShaderiv(m_shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);

            std::vector<char> infoLog(infoLogLength);
            glGetShaderInfoLog(m_shaderId, infoLogLength, nullptr, infoLog.data());

            m_compileError = std::string(infoLog.begin(), infoLog.end());
            m_isCompiled = false;

            glDeleteShader(m_shaderId);
            m_shaderId = 0;

            return false;
        }

        m_isCompiled = true;
        m_compileError.clear();
        return true;
    }

} // namespace FCT