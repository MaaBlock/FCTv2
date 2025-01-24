#pragma once
#include "Material.h"
#include "GL_VertexShader.h"
#include "GL_PixelShader.h"
#include <glad/glad.h>

namespace FCT {

class GL_Material : public Material {
public:
    GL_Material(GL_VertexShader* vertexShader, GL_PixelShader* pixelShader);
    ~GL_Material();

    bool compile() override;
    void bind() override;
    void unbind() override;

    VertexShader* getVertexShader() const override { return m_vertexShader; }
    PixelShader* getPixelShader() const override { return m_pixelShader; }

private:
    GLuint m_programID;
    bool m_isCompiled;
};

} // namespace FCT
