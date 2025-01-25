#pragma once
#include "./Shape.h"
#include "../Context/DataTypes.h"
#include "../Context/VertexFactory.h"
#include "../Context/VertexArray.h"
#include "../Context/VertexBuffer.h"

namespace FCT {
    class Triangle : public Shape {
    public:
        Triangle(Context* context, VertexFactory* factory) : m_factory(factory) {
            m_context = context;
            m_array = new VertexArray(m_factory, 3);
        }

        Triangle(Context* context, VertexFactory* factory, const Vec2& size) : m_factory(factory) {
            m_context = context;
            m_array = new VertexArray(m_factory, 3);
            this->size(size);
        }

        void vertex(unsigned vertexIndex,const Vec2& vertex) {
            m_array->setPosition(vertexIndex, vertex);
        }

        void vertices(const Vec2& v1, const Vec2& v2, const Vec2& v3) {
            m_array->setPosition(0, v1);
            m_array->setPosition(1, v2);
            m_array->setPosition(2, v3);
        }

        void color(const Vec4& color) {
            m_array->setColor(0, color);
            m_array->setColor(1, color);
            m_array->setColor(2, color);
        }

        void color(unsigned vertexIndex,const Vec4& color) {
            m_array->setColor(vertexIndex, color);
        }

		void color(const Vec4& color1, const Vec4& color2, const Vec4& color3) {
			m_array->setColor(0, color1);
			m_array->setColor(1, color2);
			m_array->setColor(2, color3);
		}

        void create() {
            m_vertexBuffer = m_context->createVertexBuffer(m_array);
            m_vertexBuffer->create(m_context);
            m_inputLayout = m_context->createInputLayout(m_factory);
            m_inputLayout->create(m_context, m_vertexBuffer);
            m_drawCall = m_context->createDrawCall(PrimitiveType::Triangles, 0, 3);
            addResource(m_vertexBuffer);
            addResource(m_inputLayout);
            addResource(m_drawCall);
            m_initialized = true;
        }

    private:
        Context* m_context;
        VertexFactory* m_factory;
        VertexArray* m_array;
        VertexBuffer* m_vertexBuffer;
        InputLayout* m_inputLayout;
        DrawCall* m_drawCall;
    };
}