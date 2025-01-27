#pragma once
#include <glad/glad.h>
#include "./MutilThreadBase/RefCount.h"
#include "./UI/Window.h"
#include "./UI/GLFW_WindowShareData.h"
#include "./UI/GLFW_Window.h"
#include "./UI/FCTKeyCode.h"
#include "./ImageLoader/ImageLoader.h"
#include "./ImageLoader/FreeImage_ImageLoader.h"
#include "./Context/IRenderTarget.h"
#include "./Context/Context.h"
#include "./Context/GL_Context.h"
#include "./Context/GL_ContextShareData.h"
#include "./Context/D3D11_Context.h"
#include "./Context/ConstBuffer.h"
#include "./Context/VertexFactory.h"
#include "./Context/VertexArray.h"
#include "./Context/VertexBuffer.h"
#include "./Context/VertexShader.h"
#include "./Context/PixelShader.h"
#include "./Context/InputLayout.h"
#include "./Context/DrawCall.h"
#include "./Context/Texture.h"
#include "./Context/GL_ConstBuffer.h"
#include "./Context/GL_VertexBuffer.h"
#include "./Context/GL_VertexShader.h"
#include "./Context/GL_PixelShader.h"
#include "./Context/GL_InputLayout.h"
#include "./Context/GL_DrawCall.h"
#include "./Context/GL_Texture.h"
#include "./Context/Pipeline.h"
#include "./Shapes/Shape.h"
#include "./Shapes/Rectangle.h"
#include "./Shapes/Triangle.h"
#include "./Object/Object.h"
#include "./Object/Box.h"
#include "./Runtime/runtime.h"
#pragma comment(lib,"FCT.lib")
