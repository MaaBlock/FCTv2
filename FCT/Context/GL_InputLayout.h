#pragma once
#include "InputLayout.h"
#include "VertexFactory.h"
#include <glad/glad.h>

namespace FCT {

class GL_InputLayout : public InputLayout {
public:
    GL_InputLayout(VertexFactory* factory);
    virtual ~GL_InputLayout();

    void bind() override;
    void unbind() override;

    bool create(Context* context, VertexBuffer* buffer = nullptr) override;

private:
    VertexFactory* m_factory;
    VertexBuffer* m_buffer;
    GLuint m_vao;
    bool m_isCreated;
};

} // namespace FCT
