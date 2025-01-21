#pragma once
#include "VertexShader.h"
#include <glad/glad.h>

namespace FCT {
    class GL_VertexShader : public VertexShader {
    public:
        GL_VertexShader(VertexFactory* factory);
        virtual ~GL_VertexShader();

        virtual bool compile() override;

    private:
        GLuint m_shaderId;
    };
} // namespace FCT