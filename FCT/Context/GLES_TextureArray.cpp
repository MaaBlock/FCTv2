#include "../headers.h"
#include <stdexcept>

void FCT::GL_TextureArray::create(unsigned int width, unsigned int height, unsigned int layers, Format format)
{
    m_width = width;
    m_height = height;
	m_layer = layers;
    m_format = format;

    glGenTextures(1, &m_textureID);
    //GL_Check("GL_TextureArray::create::glGenTextures");
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_textureID);
    //GL_Check("GL_TextureArray::create::glBindTexture");

    GLenum internalFormat = getGLESInternalFormat();
    GLenum formatEnum = getGLESFormat();

	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, internalFormat, m_width, m_height, layers);
	//glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, internalFormat, m_width, m_height, layers, 0, formatEnum, GL_UNSIGNED_BYTE, nullptr);
    //GL_Check("GL_TextureArray::create::glTexImage3D");
    //glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, m_width, m_height, layers, formatEnum, GL_UNSIGNED_BYTE, nullptr);

    //GL_Check("GL_TextureArray::create::glTexSubImage3D");
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //GL_Check("GL_TextureArray::create::glTexParameteri");

    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    //GL_Check("GL_TextureArray::create::glTexParameteri");
    GL_Check("GL_TextureArray::create");
}

void FCT::GL_TextureArray::setData(unsigned int layer, const void* data, size_t size) {
    glActiveTexture(GL_TEXTURE0 + m_slot);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_textureID);
    glTexSubImage3D(
        GL_TEXTURE_2D_ARRAY,
        0,
        0, 0, layer,
        m_width,
        m_height,
        1,
        getGLESFormat(),
        GL_UNSIGNED_BYTE,
        data
    );
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    GL_Check("GL_TextureArray::setData");
}


unsigned int FCT::GL_TextureArray::getWidth() const
{
    return m_width;
}

unsigned int FCT::GL_TextureArray::getHeight() const
{
    return m_height;
}

FCT::TextureArray::Format FCT::GL_TextureArray::getFormat() const
{
    return m_format;
}

void FCT::GL_TextureArray::bind()
{
    glActiveTexture(GL_TEXTURE0 + m_slot);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_textureID);
    GL_Check("FCT::GL_TextureArray::bind");
}

void FCT::GL_TextureArray::unbind()
{
    glActiveTexture(GL_TEXTURE0 + m_slot);
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void FCT::GL_TextureArray::setSlot(unsigned int slot)
{
    m_slot = slot;
}

unsigned int FCT::GL_TextureArray::getSlot() const
{
    return m_slot;
}

GLenum FCT::GL_TextureArray::getGLESInternalFormat() const
{
    switch (m_format) {
    case Format::R32F: return GL_R32F;
    case Format::RG32F: return GL_RG32F;
    case Format::RGB32F: return GL_RGB32F;
    case Format::RGBA32F: return GL_RGBA32F;
    case Format::RGB8: return GL_RGB8;
    case Format::RGBA8: return GL_RGBA8;
    default: throw std::runtime_error("Unsupported texture format");
    }
}

unsigned int FCT::GL_TextureArray::getLayers() const
{
	return m_layer;
}


GLenum FCT::GL_TextureArray::getGLESFormat() const
{
    switch (m_format) {
    case Format::R32F: return GL_RED;
    case Format::RG32F: return GL_RG;
    case Format::RGB32F: return GL_RGB;
    case Format::RGBA32F: return GL_RGBA;
    case Format::RGB8: return GL_RGB;
    case Format::RGBA8: return GL_RGBA;
    default: throw std::runtime_error("Unsupported texture format");
    }
}