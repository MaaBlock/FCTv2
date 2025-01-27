#pragma once
#include "Texture.h"
#include <glad/glad.h>

namespace FCT {
    class GL_Texture : public Texture {
    public:
        GL_Texture();
        virtual ~GL_Texture();

        GLenum getGLDataType() const;

        void create(unsigned int width, unsigned int height, Format format) override;
        void setData(const void* data, size_t size) override;

        unsigned int getWidth() const override { return m_width; }
        unsigned int getHeight() const override { return m_height; }
        Format getFormat() const override { return m_format; }

        void bind() override;
        void unbind() override;

        void setSlot(unsigned int slot) override;
        unsigned int getSlot() const override;
    private:
        GLuint m_textureID;
        unsigned int m_width;
        unsigned int m_height;
        Format m_format;
		unsigned int m_slot;
        GLenum getGLFormat() const;
        GLenum getGLInternalFormat() const;

    };
}