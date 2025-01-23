#include "../headers.h"
#include <iostream>

namespace FCT {

    Pipeline::Pipeline(Context* context, VertexFactory* defaultFactory)
        : m_context(context), m_defaultVertexShader(nullptr), m_defaultPixelShader(nullptr), m_defaultFactory(defaultFactory) {
        createDefaultShaders();
    }

    Pipeline::~Pipeline() {
        delete m_defaultVertexShader;
        delete m_defaultPixelShader;
    }

    VertexShader* Pipeline::getDefaultVertexShader() const {
        return m_defaultVertexShader;
    }

    PixelShader* Pipeline::getDefaultPixelShader() const {
        return m_defaultPixelShader;
    }

    void Pipeline::createDefaultShaders() {
        m_defaultVertexShader = m_context->createVertexShader(m_defaultFactory);
        if (!m_defaultVertexShader->compileFromSource("")) {
            std::cout << m_defaultVertexShader->getCompileError() << std::endl;
        }

        m_defaultPixelShader = m_context->createPixelShader(m_defaultVertexShader->getOutput());
		if (!m_defaultPixelShader->compileFromSource("")) {
			std::cout << m_defaultPixelShader->getCompileError() << std::endl;
		}
    }

} // namespace FCT
