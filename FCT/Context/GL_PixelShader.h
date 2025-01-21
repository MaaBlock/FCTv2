#pragma once
#include "./PixelShader.h"
#include <glad/glad.h>  // 包含OpenGL头文件

namespace FCT {

class GL_PixelShader : public PixelShader {
public:
    GL_PixelShader(const ShaderOutput& vertexOutput);
    virtual ~GL_PixelShader();

    virtual bool compile() override;

private:
    GLuint m_shaderID;
};

} // namespace FCT