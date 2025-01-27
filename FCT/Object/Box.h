#pragma once
#include "../Object/Object.h"
#include "../Context/DataTypes.h"
#include "../Context/VertexFactory.h"
#include "../Context/VertexArray.h"
#include "../Context/VertexBuffer.h"

namespace FCT {
	class Box : public Object {
	public:
		Box(Context* context, VertexFactory* factory);
		Box(Context* context, VertexFactory* factory, const Vec3& size);

		void size(const Vec3& size);
		void color(const Vec4& color);
		virtual void create() override;

	private:
		Context* m_context;
		VertexFactory* m_factory;
		VertexArray* m_array;
		VertexBuffer* m_vertexBuffer;
		InputLayout* m_inputLayout;
		DrawCall* m_drawCall;
		Vec3 m_size;

		void updateVertices();
	};
}