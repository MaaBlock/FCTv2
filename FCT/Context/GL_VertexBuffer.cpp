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
    GL_Check("before glGenBuffers");
    if (m_bufferId == 0) {
        return false;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_bufferId); 
    GL_Check("before glBindBuffer");
    glBufferData(GL_ARRAY_BUFFER, m_vertexArray->getByteSize(), m_vertexArray->data(), GL_DYNAMIC_DRAW);
    GL_Check("before glBufferData");
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    m_isCreated = true;
	GL_Check("before create vertex buffer");
    return true;
}

void GL_VertexBuffer::updata()
{
	if (m_isCreated) {
		glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
		glBufferData(GL_ARRAY_BUFFER, m_vertexArray->getByteSize(), m_vertexArray->data(), GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

} // namespace FCT
