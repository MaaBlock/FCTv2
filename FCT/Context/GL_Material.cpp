#include "GL_Material.h"
#include <iostream>

namespace FCT {

GL_Material::GL_Material(GL_VertexShader* vertexShader, GL_PixelShader* pixelShader)
    : Material(vertexShader, pixelShader), m_programID(0), m_isCompiled(false)
{
}

GL_Material::~GL_Material()
{
    if (m_programID != 0) {
        glDeleteProgram(m_programID);
    }
}

bool GL_Material::compile()
{
    if (m_programID != 0) {
        glDeleteProgram(m_programID);
    }

    // Create program
    m_programID = glCreateProgram();
    if (m_programID == 0) {
        return false;
    }

    // Attach shaders
    auto glVertexShader = dynamic_cast<GL_VertexShader*>(m_vertexShader);
    auto glPixelShader = dynamic_cast<GL_PixelShader*>(m_pixelShader);

    if (glVertexShader && glVertexShader->isCompiled()) {
        glAttachShader(m_programID, glVertexShader->getShaderID());
    }
    if (glPixelShader && glPixelShader->isCompiled()) {
        glAttachShader(m_programID, glPixelShader->getShaderID());
    }

    // Link program
    glLinkProgram(m_programID);

    // Check link status
    GLint linkStatus;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE) {
        GLint infoLogLength;
        glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);

        std::vector<char> infoLog(infoLogLength);
        glGetProgramInfoLog(m_programID, infoLogLength, nullptr, infoLog.data());

        std::cerr << "Program link failed: " << std::string(infoLog.begin(), infoLog.end()) << std::endl;
        return false;
    }

    m_isCompiled = true;
    return true;
}

void GL_Material::bind()
{
    if (m_isCompiled) {
        glUseProgram(m_programID);
    }
}

void GL_Material::unbind()
{
    glUseProgram(0);
}

} // namespace FCT
