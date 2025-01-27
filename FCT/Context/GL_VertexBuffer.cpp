#include "../headers.h"
namespace FCT {

GL_VertexBuffer::GL_VertexBuffer(VertexArray* vertexArray)
    : m_vertexArray(vertexArray), m_bufferId(0), m_isCreated(false) {
}

GL_VertexBuffer::~GL_VertexBuffer() {
    if (m_bufferId != 0) {
        glDeleteBuffers(1, &m_bufferId);
    }
}

void GL_VertexBuffer::bind() {
    if (m_isCreated) {
        glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
    }
}

void GL_VertexBuffer::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool GL_VertexBuffer::create(Context* context) {
    if (m_isCreated) {
        return true;
    }

    glGenBuffers(1, &m_bufferId);
    if (m_bufferId == 0) {
        return false;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
    glBufferData(GL_ARRAY_BUFFER, m_vertexArray->getSize(), m_vertexArray->getData(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    m_isCreated = true;
    return true;
}

} // namespace FCT
