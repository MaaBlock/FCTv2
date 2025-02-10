#pragma once
#include "./TextureArray.h"
#include <glad/glad.h>
namespace FCT {
	class GL_TextureArray : public TextureArray {
	public:
		void create(unsigned int width, unsigned int height,unsigned int layers, Format format) override;
		void setData(unsigned int layer ,const void* data, size_t size) override;
		unsigned int getWidth() const override;
		unsigned int getHeight() const override;
		Format getFormat() const override;
		void bind() override;
		void unbind() override;
		void setSlot(unsigned int slot) override;
		unsigned int getSlot() const override;
		unsigned int getLayers() const override;
	private:
		GLuint m_textureID;
		unsigned int m_width;
		unsigned int m_height;
		unsigned int m_layer;
		Format m_format;
		unsigned int m_slot;
		GLenum getGLESFormat() const;
		GLenum getGLESInternalFormat() const;

	};
}