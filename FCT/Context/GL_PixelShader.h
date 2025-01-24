#pragma once
#include "./PixelShader.h"
#include <glad/glad.h> 

namespace FCT {

class GL_PixelShader : public PixelShader {
public:
    GL_PixelShader(const ShaderOutput& vertexOutput);
    virtual ~GL_PixelShader();
    virtual bool compile() override;
    void bind() override;
    void unbind() override;
    GLuint getShaderID() const { return m_shaderID; }
private:
    GLuint m_shaderID;
};

} // namespace FCT
