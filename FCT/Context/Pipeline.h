#pragma once
#include "./VertexShader.h"
#include "./PixelShader.h"
#include "./VertexFactory.h"

namespace FCT {

    class Context;

    class Pipeline {
    public:
        Pipeline(Context* context, VertexFactory* defaultFactory);
        ~Pipeline();

        VertexShader* getDefaultVertexShader() const;
        PixelShader* getDefaultPixelShader() const;

    private:
        Context* m_context;
        VertexShader* m_defaultVertexShader;
        PixelShader* m_defaultPixelShader;
        VertexFactory* m_defaultFactory;

        void createDefaultShaders();
    };

} // namespace FCT
