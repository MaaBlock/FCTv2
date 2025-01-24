#include "GL_InputLayout.h"
#include "Context.h"
#include <iostream>

namespace FCT {

GL_InputLayout::GL_InputLayout(VertexFactory* factory)
    : m_factory(factory), m_vao(0), m_isCreated(false) {
}

GL_InputLayout::~GL_InputLayout() {
    if (m_vao != 0) {
        glDeleteVertexArrays(1, &m_vao);
    }
}

void GL_InputLayout::bind() {
    if (m_isCreated) {
        glBindVertexArray(m_vao);
    }
}

void GL_InputLayout::unbind() {
    if (m_isCreated) {
        glBindVertexArray(0);
    }
}

bool GL_InputLayout::create(Context* context) {
    if (m_isCreated) {
        return true;
    }

    glGenVertexArrays(1, &m_vao);
    if (m_vao == 0) {
        return false;
    }

    glBindVertexArray(m_vao);

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL Error affter glBindVertexArray: " << err << std::endl;
    }
    int locationCounter = 0;
    for (const auto& attr : m_factory->getAttributes()) {
        GLint size = 0;
        GLenum type = GL_FLOAT;
        GLboolean normalized = GL_FALSE;

        switch (attr.dataType) {
            case DataType::Float:
                size = 1;
                type = GL_FLOAT;
                break;
            case DataType::Vec2:
                size = 2;
                type = GL_FLOAT;
                break;
            case DataType::Vec3:
                size = 3;
                type = GL_FLOAT;
                break;
            case DataType::Vec4:
                size = 4;
                type = GL_FLOAT;
                break;
            case DataType::Mat2:
                size = 4; // 2x2 matrix
                type = GL_FLOAT;
                break;
            case DataType::Mat3:
                size = 9; // 3x3 matrix
                type = GL_FLOAT;
                break;
            case DataType::Mat4:
                size = 16; // 4x4 matrix
                type = GL_FLOAT;
                break;
            case DataType::Int:
                size = 1;
                type = GL_INT;
                break;
            case DataType::IVec2:
                size = 2;
                type = GL_INT;
                break;
            case DataType::IVec3:
                size = 3;
                type = GL_INT;
                break;
            case DataType::IVec4:
                size = 4;
                type = GL_INT;
                break;
            case DataType::UInt:
                size = 1;
                type = GL_UNSIGNED_INT;
                break;
            case DataType::UVec2:
                size = 2;
                type = GL_UNSIGNED_INT;
                break;
            case DataType::UVec3:
                size = 3;
                type = GL_UNSIGNED_INT;
                break;
            case DataType::UVec4:
                size = 4;
                type = GL_UNSIGNED_INT;
                break;
            case DataType::Bool:
                size = 1;
                type = GL_BOOL;
                break;
            case DataType::BVec2:
                size = 2;
                type = GL_BOOL;
                break;
            case DataType::BVec3:
                size = 3;
                type = GL_BOOL;
                break;
            case DataType::BVec4:
                size = 4;
                type = GL_BOOL;
                break;
            default:
                throw std::runtime_error("Unsupported data type");
        }

        glEnableVertexAttribArray(locationCounter);
        glVertexAttribPointer(locationCounter, size, type, normalized, m_factory->getStride(), reinterpret_cast<void*>(attr.offset));

        locationCounter++;
    }

    glBindVertexArray(0);

    m_isCreated = true;
    return true;
}

} // namespace FCT

