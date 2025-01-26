#pragma once
#include "ConstBuffer.h"
#include "Context.h"

namespace FCT {

class GL_ConstBuffer : public ConstBuffer {
public:
    GL_ConstBuffer(Context* context);
    virtual ~GL_ConstBuffer();

    bool create(size_t size, uint32_t bindIndex) override;
    void setData(const void* data, size_t size, size_t offset = 0) override;
    void bind() override;
    void unbind() override;

private:
    Context* m_context;
    uint32_t m_bufferId;
};

} // namespace FCT