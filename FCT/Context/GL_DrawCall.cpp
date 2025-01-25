#include "../headers.h"

namespace FCT {
    GL_DrawCall::GL_DrawCall(PrimitiveType primitiveType, uint32_t startVertex, uint32_t vertexCount)
        : DrawCall(primitiveType, startVertex, vertexCount) {

    }

    void GL_DrawCall::bind() {
        GLenum glPrimitiveType = convertPrimitiveType(m_primitiveType);
        glDrawArrays(glPrimitiveType, m_startVertex, m_vertexCount);
    }

    void GL_DrawCall::unbind() {

    }

    GLenum GL_DrawCall::convertPrimitiveType(PrimitiveType type) const {
        switch (type) {
        case PrimitiveType::Lines:
            return GL_LINES;
        case PrimitiveType::Triangles:
            return GL_TRIANGLES;
        default:
            throw std::runtime_error("Unsupported primitive type");
        }
    }
}