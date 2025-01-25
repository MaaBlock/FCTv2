#include "../headers.h"
#include <iostream>

namespace FCT {

    Pipeline::Pipeline(Context* context, VertexFactory* defaultFactory)
        : m_context(context), m_defaultVertexShader(nullptr), m_defaultPixelShader(nullptr), m_defaultFactory(defaultFactory) {
        createDefaultResource();
    }

    Pipeline::~Pipeline() {
        delete m_defaultVertexShader;
        delete m_defaultPixelShader;
    }

    void Pipeline::begin()
    {
        m_defaultMaterial->bind();
	}

    void Pipeline::end()
    {
    }

    void Pipeline::draw(Shape* shape)
    {
		shape->draw();
	}

    VertexShader* Pipeline::getDefaultVertexShader() const {
        return m_defaultVertexShader;
    }

    PixelShader* Pipeline::getDefaultPixelShader() const {
        return m_defaultPixelShader;
    }

    Material* Pipeline::getDefaultMaterial() const
    {
		return m_defaultMaterial;
    }

    void Pipeline::createDefaultResource() {
        m_defaultVertexShader = m_context->createVertexShader(m_defaultFactory);
        if (!m_defaultVertexShader->compileFromSource("")) {
            std::cout << m_defaultVertexShader->getCompileError() << std::endl;
        }

        m_defaultPixelShader = m_context->createPixelShader(m_defaultVertexShader->getOutput());
		if (!m_defaultPixelShader->compileFromSource("")) {
			std::cout << m_defaultPixelShader->getCompileError() << std::endl;
		}

		m_defaultMaterial = m_context->createMaterial(m_defaultVertexShader, m_defaultPixelShader);
        m_defaultMaterial->compile();
    }

} // namespace FCT
