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

    FCT::VertexBuffer* vertexBuffer = ctx->createVertexBuffer(vertexArray);
    vertexBuffer->create(ctx);

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL Error before create InputLayout: " << err << std::endl;
    }
    FCT::InputLayout* inputLayout = ctx->createInputLayout(factory);
    vertexBuffer->bind();
    inputLayout->create(ctx);
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

    while (wnd->isRunning()) {
        ctx->clear(0.2f, 0.3f, 0.3f);
        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cerr << "OpenGL Error before bind: " << err << std::endl;
        }
        inputLayout->bind();
        vertexBuffer->bind();
        material->bind();

        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cerr << "OpenGL Error after bind: " << err << std::endl;
        }

        glDrawArrays(GL_TRIANGLES, 0, 3);

        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cerr << "OpenGL Error after draw: " << err << std::endl;
        }

        wnd->swapBuffers();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    delete vertexArray;
    delete vertexBuffer;
    delete vs;
    delete ps;
    delete factory;

    rt->tern();
    return 0;
}
