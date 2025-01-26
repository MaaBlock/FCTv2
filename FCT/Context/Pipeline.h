#pragma once
#include "./VertexShader.h"
#include "./PixelShader.h"
#include "./VertexFactory.h"
#include "../Shapes/Shape.h"

namespace FCT {

    class Context;

    class Pipeline {
    public:
        Pipeline(Context* context, VertexFactory* defaultFactory);
        ~Pipeline();
        void begin();
		void end();
        void draw(Shape* shape);
        VertexShader* getDefaultVertexShader() const;
        PixelShader* getDefaultPixelShader() const;
		Material* getDefaultMaterial() const;

    private:
        Context* m_context;
        VertexShader* m_defaultVertexShader;
        PixelShader* m_defaultPixelShader;
		Material* m_defaultMaterial;
        VertexFactory* m_defaultFactory;
        ConstBuffer* m_viewMatrixConstBuffer;
		ConstBuffer* m_projectionMatrixConstBuffer;
        Mat4 m_viewMatrix;
		Mat4 m_projectionMatrix;
        void createDefaultResource();
    };

} // namespace FCT
