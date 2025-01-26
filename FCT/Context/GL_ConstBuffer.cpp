#include "../headers.h"
namespace FCT {

	GL_ConstBuffer::GL_ConstBuffer(Context* context)
		: m_context(context), m_bufferId(0)
	{
	}

	GL_ConstBuffer::~GL_ConstBuffer()
	{
		if (m_bufferId != 0) {
			glDeleteBuffers(1, &m_bufferId);
		}
	}

	bool GL_ConstBuffer::create(size_t size, uint32_t bindIndex)
	{
		m_size = size;
		m_bindIndex = bindIndex;

		glGenBuffers(1, &m_bufferId);
		glBindBuffer(GL_UNIFORM_BUFFER, m_bufferId);
		glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBufferBase(GL_UNIFORM_BUFFER, m_bindIndex, m_bufferId);

		return true;
	}

	void GL_ConstBuffer::setData(const void* data, size_t size, size_t offset)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_bufferId);
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void GL_ConstBuffer::bind()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_bufferId);
	}

	void GL_ConstBuffer::unbind()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

} // namespace FCT