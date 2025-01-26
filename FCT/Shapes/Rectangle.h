#pragma once
#include "./Shape.h"
#include "../Context/DataTypes.h"
#include "../Context/VertexFactory.h"
#include "../Context/VertexArray.h"
#include "../Context/VertexBuffer.h"
namespace FCT {
	class Rectangle : public Shape {
	public:
		Rectangle(Context* context, VertexFactory* factory) : m_factory(factory), Shape(context){
			m_context = context;
			m_array = new VertexArray(m_factory, 6);
		}
		Rectangle(Context* context, VertexFactory* factory, const Vec2& size) : m_factory(factory), Shape(context) {
			m_context = context;
			m_array = new VertexArray(m_factory, 6);
			this->size(size);
		}
		void size(const Vec2& size) {
			m_array->setPosition(0, Vec2(-size.x / 2, -size.y / 2));
			m_array->setPosition(1, Vec2(size.x / 2, -size.y / 2));
			m_array->setPosition(2, Vec2(size.x / 2, size.y / 2));
			m_array->setPosition(3, Vec2(-size.x / 2, -size.y / 2));
			m_array->setPosition(4, Vec2(size.x / 2, size.y / 2));
			m_array->setPosition(5, Vec2(-size.x / 2, size.y / 2));
		}
		void color(const Vec4& color) {
			m_array->setColor(0, color);
			m_array->setColor(1, color);
			m_array->setColor(2, color);
			m_array->setColor(3, color);
			m_array->setColor(4, color);
			m_array->setColor(5, color);
		}
		void create() {
			m_vertexBuffer = m_context->createVertexBuffer(m_array);
			m_vertexBuffer->create(m_context);
			m_inputLayout = m_context->createInputLayout(m_factory);
			m_inputLayout->create(m_context, m_vertexBuffer);
			m_drawCall = m_context->createDrawCall(PrimitiveType::Triangles, 0, 6);
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