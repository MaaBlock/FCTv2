#pragma once
#include "../MutilThreadBase/RefCount.h" 
#include "./VertexFactory.h"
#include "./VertexShader.h"
#include "./PixelShader.h"
namespace FCT {
	class Context : public RefCount {
	public:
		virtual ~Context() {};
		virtual void clear(float r, float g, float b) = 0;
		virtual void viewport(int x, int y, int width, int height) = 0;
		virtual VertexShader* createVertexShader(VertexFactory* factory) = 0;
		virtual PixelShader* createPixelShader(const ShaderOutput& output) = 0;
	private:

	};
}