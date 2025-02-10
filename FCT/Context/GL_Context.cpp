#include "../headers.h"
namespace FCT {
	GL_Context::GL_Context(GL_ContextShareData* shareData, Runtime* runtime)
	{
		g_shareData = shareData;
		g_runtime = runtime;
		g_glVersionMajor = g_shareData->getGLVersionMajor();
		g_glVersionMinor = g_shareData->getGLVersionMinor();
	}

	void GL_Context::create(IRenderTarget* target)
	{
		m_target = target;
		target->bind(this);
	}

	void GL_Context::clear(float r, float g, float b)
	{
		glClearColor(r, g, b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);

	}

	void GL_Context::viewport(int x, int y, int width, int height)
	{
		glViewport(x, y, width, height);
	}

	VertexShader* GL_Context::createVertexShader(VertexFactory* factory)
	{
		return new GL_VertexShader(factory);
	}

	PixelShader* GL_Context::createPixelShader(const ShaderOutput& output)
	{
		return new GL_PixelShader(output);
	}

	VertexBuffer* GL_Context::createVertexBuffer(VertexArray* array)
	{
		return new GL_VertexBuffer(array);
	}

	Material* GL_Context::createMaterial(VertexShader* vertexShader, PixelShader* pixelShader)
	{
		return new GL_Material(
			dynamic_cast<GL_VertexShader*>(vertexShader),
			dynamic_cast<GL_PixelShader*>(pixelShader)
		);
	}

	InputLayout* GL_Context::createInputLayout(VertexFactory* factory)
	{
		return new GL_InputLayout(factory);
	}

	DrawCall* GL_Context::createDrawCall(PrimitiveType primitiveType, uint32_t startVertex, uint32_t vertexCount)
	{
		return new GL_DrawCall(primitiveType, startVertex, vertexCount);
	}

	Texture* GL_Context::createTexture()
	{
		return new GL_Texture();
	}

	ConstBuffer* GL_Context::createConstBuffer()
	{
		return new GL_ConstBuffer(this);
	}

	TextureArray* GL_Context::createTextureArray()
	{
		return new GL_TextureArray();
	}

}
