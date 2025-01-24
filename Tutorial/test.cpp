#include "../FCT/headers.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <thread>
#include <iostream>

int main() {
    FCT::Runtime* rt = FCT::CreateRuntime();
    rt->init();
    rt->setOpenGLVesion(3, 3);
    auto wnd = rt->createWindow(800, 600, "Triangle Example");
    auto ctx = rt->createContext(wnd);

    FCT::VertexFactory* factory = new FCT::VertexFactory;
    factory->addAttribute(FCT::PipelineAttributeType::Position2f, "aPos");
    factory->addAttribute(FCT::PipelineAttributeType::Color4f, "aColor");
    FCT::VertexArray* vertexArray = new FCT::VertexArray(*factory, 3);
    vertexArray->setAttribute(0, "aPos", FCT::Vec2(-0.5f, -0.5f));
    vertexArray->setAttribute(0, "aColor", FCT::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
    vertexArray->setAttribute(1, "aPos", FCT::Vec2(0.5f, -0.5f));
    vertexArray->setAttribute(1, "aColor", FCT::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
    vertexArray->setAttribute(2, "aPos", FCT::Vec2(0.0f, 0.5f));
    vertexArray->setAttribute(2, "aColor", FCT::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
    FCT::VertexArray* bgArray = new FCT::VertexArray(*factory, 4);
    bgArray->setAttribute(0, "aPos", FCT::Vec2(-1.0f, -1.0f));
    bgArray->setAttribute(0, "aColor", FCT::Vec4(0.2f, 0.3f, 0.3f, 1.0f));
    bgArray->setAttribute(1, "aPos", FCT::Vec2(1.0f, -1.0f));
    bgArray->setAttribute(1, "aColor", FCT::Vec4(0.2f, 0.3f, 0.3f, 1.0f));
    bgArray->setAttribute(2, "aPos", FCT::Vec2(1.0f, 1.0f));
    bgArray->setAttribute(2, "aColor", FCT::Vec4(0.2f, 0.3f, 0.3f, 1.0f));
    bgArray->setAttribute(3, "aPos", FCT::Vec2(-1.0f, 1.0f));
    bgArray->setAttribute(3, "aColor", FCT::Vec4(0.2f, 0.3f, 0.3f, 1.0f));
    
    FCT::VertexBuffer* bgBuffer = ctx->createVertexBuffer(bgArray);
    bgBuffer->create(ctx);

    FCT::VertexBuffer* triangleBuffer = ctx->createVertexBuffer(vertexArray);
    triangleBuffer->create(ctx);

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL Error before create InputLayout: " << err << std::endl;
    }
    FCT::InputLayout* triangleInputLayout = ctx->createInputLayout(factory);
    triangleInputLayout->create(ctx, triangleBuffer);

    FCT::InputLayout* bgInputLayout = ctx->createInputLayout(factory);
    bgInputLayout->create(ctx, bgBuffer);
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL Error after create InputLayout: " << err << std::endl;
    }
    FCT::VertexShader* vs = ctx->createVertexShader(factory);
    if (!vs->compileFromSource("")) {
        std::cout << "vs报错：" << std::endl;
        std::cout << vs->getCompileError() << std::endl;
        std::cout << "vs源码：" << std::endl;
        std::cout << vs->getSource() << std::endl;
    }
    FCT::PixelShader* ps = ctx->createPixelShader(vs->getOutput());
    if (!ps->compileFromSource(
        "")) {
        std::cout << "ps报错：" << std::endl;
        std::cout << ps->getCompileError() << std::endl;
        std::cout << "ps源码：" << std::endl;
        std::cout << ps->getSource() << std::endl;
    }
    auto material = ctx->createMaterial(vs, ps);
    material->compile();
    ctx->viewport(0, 0, 800, 600);
    bool needViewPort = false;
    wnd->getCallBack()->addResizeCallback([&needViewPort](FCT::Window* wnd, int w, int h) {
        needViewPort = true;
        });
    while (wnd->isRunning()) {
        if (needViewPort) {
            float x = 0, y = 0, w = wnd->getWidth(), h = wnd->getHeight();
            if (h / w > 0.75) {
                x = 0;
                y = (h - w * 3 / 4) / 2;
                h = w * 3 / 4;
            }
            else {
                x = (w - h * 4 / 3) / 2;
                y = 0;
                w = h * 4 / 3;
            }
            ctx->viewport(x, y, w, h);
        }
        glClear(GL_COLOR_BUFFER_BIT);
        ctx->clear(0, 0, 0);

        material->bind();
        bgBuffer->bind();
        bgInputLayout->bind();
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        triangleBuffer->bind();
        triangleInputLayout->bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);

        wnd->swapBuffers();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    delete vertexArray;
    delete triangleBuffer;
    delete vs;
    delete ps;
    delete factory;

    rt->tern();
    return 0;
}
