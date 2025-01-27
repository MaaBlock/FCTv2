#include "../headers.h"

namespace FCT {
	Box::Box(Context* context, VertexFactory* factory)
		: m_factory(factory), Object(context), m_size(1, 1, 1) {
		m_context = context;
		m_array = new VertexArray(m_factory, 36); 
		m_texture = nullptr;
		updateVertices();
	}

	Box::Box(Context* context, VertexFactory* factory, const Vec3& size)
		: m_factory(factory), Object(context), m_size(size) {
		m_context = context;
		m_array = new VertexArray(m_factory, 36);
		m_texture = nullptr;
		updateVertices();
	}
	Box::~Box()
	{
		safeRelease(m_texture);
		safeRelease(m_inputLayout);
		safeRelease(m_vertexBuffer);
		safeRelease(m_drawCall);
	}
	void Box::texture(Texture* texture)
	{
		safeRelease(m_texture);
		m_texture = texture;
		m_texture->setSlot(3);
		safeAddRef(m_texture);
	}
	void Box::setTextureCoordinates() {
		// Front face
		m_array->setTexCoord(0, Vec2(0, 1));
		m_array->setTexCoord(1, Vec2(1, 1));
		m_array->setTexCoord(2, Vec2(1, 0));
		m_array->setTexCoord(3, Vec2(0, 1));
		m_array->setTexCoord(4, Vec2(1, 0));
		m_array->setTexCoord(5, Vec2(0, 0));

		// Back face
		m_array->setTexCoord(6, Vec2(0, 1));
		m_array->setTexCoord(7, Vec2(1, 1));
		m_array->setTexCoord(8, Vec2(1, 0));
		m_array->setTexCoord(9, Vec2(0, 1));
		m_array->setTexCoord(10, Vec2(1, 0));
		m_array->setTexCoord(11, Vec2(0, 0));

		// Top face
		m_array->setTexCoord(12, Vec2(0, 1));
		m_array->setTexCoord(13, Vec2(0, 0));
		m_array->setTexCoord(14, Vec2(1, 0));
		m_array->setTexCoord(15, Vec2(0, 1));
		m_array->setTexCoord(16, Vec2(1, 0));
		m_array->setTexCoord(17, Vec2(1, 1));

		// Bottom face
		m_array->setTexCoord(18, Vec2(0, 0));
		m_array->setTexCoord(19, Vec2(1, 0));
		m_array->setTexCoord(20, Vec2(1, 1));
		m_array->setTexCoord(21, Vec2(0, 0));
		m_array->setTexCoord(22, Vec2(1, 1));
		m_array->setTexCoord(23, Vec2(0, 1));

		// Right face
		m_array->setTexCoord(24, Vec2(0, 1));
		m_array->setTexCoord(25, Vec2(0, 0));
		m_array->setTexCoord(26, Vec2(1, 0));
		m_array->setTexCoord(27, Vec2(0, 1));
		m_array->setTexCoord(28, Vec2(1, 0));
		m_array->setTexCoord(29, Vec2(1, 1));

		// Left face
		m_array->setTexCoord(30, Vec2(1, 1));
		m_array->setTexCoord(31, Vec2(0, 1));
		m_array->setTexCoord(32, Vec2(0, 0));
		m_array->setTexCoord(33, Vec2(1, 1));
		m_array->setTexCoord(34, Vec2(0, 0));
		m_array->setTexCoord(35, Vec2(1, 0));
	}
	void Box::updata()
	{
		m_vertexBuffer->updata();
	}
	void Box::size(const Vec3& size) {
		m_size = size;
		updateVertices();
	}

	void Box::color(const Vec4& color) {
		for (int i = 0; i < 36; ++i) {
			m_array->setColor(i, color);
		}
	}

	void Box::create() {
		m_vertexBuffer = m_context->createVertexBuffer(m_array);
		m_vertexBuffer->create(m_context);
		m_inputLayout = m_context->createInputLayout(m_factory);
		m_inputLayout->create(m_context, m_vertexBuffer);
		m_drawCall = m_context->createDrawCall(PrimitiveType::Triangles, 0, 36);
		addResource(m_vertexBuffer);
		addResource(m_inputLayout);
		addResource(m_texture);
		addResource(m_drawCall);
		m_initialized = true;
	}

	void Box::updateVertices() {
		float w = m_size.x * 0.5f;
		float h = m_size.y * 0.5f;
		float d = m_size.z * 0.5f;

		// Front face
		m_array->setPosition(0, Vec3(-w, -h, d));
		m_array->setPosition(1, Vec3(w, -h, d));
		m_array->setPosition(2, Vec3(w, h, d));
		m_array->setPosition(3, Vec3(-w, -h, d));
		m_array->setPosition(4, Vec3(w, h, d));
		m_array->setPosition(5, Vec3(-w, h, d));

		// Back face
		m_array->setPosition(6, Vec3(w, -h, -d));
		m_array->setPosition(7, Vec3(-w, -h, -d));
		m_array->setPosition(8, Vec3(-w, h, -d));
		m_array->setPosition(9, Vec3(w, -h, -d));
		m_array->setPosition(10, Vec3(-w, h, -d));
		m_array->setPosition(11, Vec3(w, h, -d));

		// Top face
		m_array->setPosition(12, Vec3(-w, h, -d));
		m_array->setPosition(13, Vec3(-w, h, d));
		m_array->setPosition(14, Vec3(w, h, d));
		m_array->setPosition(15, Vec3(-w, h, -d));
		m_array->setPosition(16, Vec3(w, h, d));
		m_array->setPosition(17, Vec3(w, h, -d));

		// Bottom face
		m_array->setPosition(18, Vec3(-w, -h, -d));
		m_array->setPosition(19, Vec3(w, -h, -d));
		m_array->setPosition(20, Vec3(w, -h, d));
		m_array->setPosition(21, Vec3(-w, -h, -d));
		m_array->setPosition(22, Vec3(w, -h, d));
		m_array->setPosition(23, Vec3(-w, -h, d));

		// Right face
		m_array->setPosition(24, Vec3(w, -h, -d));
		m_array->setPosition(25, Vec3(w, h, -d));
		m_array->setPosition(26, Vec3(w, h, d));
		m_array->setPosition(27, Vec3(w, -h, -d));
		m_array->setPosition(28, Vec3(w, h, d));
		m_array->setPosition(29, Vec3(w, -h, d));

		// Left face
		m_array->setPosition(30, Vec3(-w, -h, -d));
		m_array->setPosition(31, Vec3(-w, -h, d));
		m_array->setPosition(32, Vec3(-w, h, d));
		m_array->setPosition(33, Vec3(-w, -h, -d));
		m_array->setPosition(34, Vec3(-w, h, d));
		m_array->setPosition(35, Vec3(-w, h, -d));
	}
}