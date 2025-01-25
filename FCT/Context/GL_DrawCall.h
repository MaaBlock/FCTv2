#pragma once
#include "DrawCall.h"
#include <glad/glad.h>

namespace FCT {
    class GL_DrawCall : public DrawCall {
    public:
        GL_DrawCall(PrimitiveType primitiveType, uint32_t startVertex, uint32_t vertexCount);
        
        void bind() override;
        void unbind() override;

    private:
        GLenum convertPrimitiveType(PrimitiveType type) const;
    };
}