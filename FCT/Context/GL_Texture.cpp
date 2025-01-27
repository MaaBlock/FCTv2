#include "../headers.h"
#include <stdexcept>

namespace FCT {

GL_Texture::GL_Texture() : m_textureID(0), m_width(0), m_height(0), m_format(Format::RGBA32F) {}

GL_Texture::~GL_Texture() {
    if (m_textureID != 0) {
        glDeleteTextures(1, &m_textureID);
    }
}

void GL_Texture::create(unsigned int width, unsigned int height, Format format) {
    m_width = width;
    m_height = height;
    m_format = format;

    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, getGLInternalFormat(), width, height, 0, getGLFormat(), GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void GL_Texture::setData(const void* data, size_t size) {
    if (m_textureID == 0) {
        throw std::runtime_error("Texture not created");
    }

    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, getGLFormat(), GL_FLOAT, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GL_Texture::bind() {
	glActiveTexture(GL_TEXTURE0 + m_slot);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void GL_Texture::unbind() {
    glActiveTexture(GL_TEXTURE0 + m_slot);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLenum GL_Texture::getGLFormat() const {
    switch (m_format) {
        case Format::R32F: return GL_RED;
        case Format::RG32F: return GL_RG;
        case Format::RGB32F: return GL_RGB;
        case Format::RGBA32F: return GL_RGBA;
        default: throw std::runtime_error("Unsupported texture format");
    }
}

GLenum GL_Texture::getGLInternalFormat() const {
    switch (m_format) {
        case Format::R32F: return GL_R32F;
        case Format::RG32F: return GL_RG32F;
        case Format::RGB32F: return GL_RGB32F;
        case Format::RGBA32F: return GL_RGBA32F;
        default: throw std::runtime_error("Unsupported texture format");
    }
}

void GL_Texture::setSlot(unsigned int slot)
{
	m_slot = slot;
}

unsigned int GL_Texture::getSlot() const
{
	return m_slot;
}

} // namespace FCT