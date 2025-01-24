#pragma once
#include "./VertexBuffer.h"
#include "./VertexFactory.h"
#include <glad/glad.h>

namespace FCT {

class GL_VertexBuffer : public VertexBuffer {
public:
    GL_VertexBuffer(VertexArray* vertexArray);
    virtual ~GL_VertexBuffer();

    void bind() override;
    void unbind() override;
    bool create(Context* context) override;

    VertexArray* getVertexArray() const { return m_vertexArray; }
    size_t getVertexCount() const { return m_vertexArray->getVertexCount(); }
    const VertexFactory* getFactory() const { return m_vertexArray->getFactory(); }
private:
    VertexArray* m_vertexArray;
    GLuint m_bufferId;
    bool m_isCreated;
};

} // namespace FCT
