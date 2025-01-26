#include "../FCT/headers.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <thread>
#include <iostream>
void checkOpenGLErrors(const char* location) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        std::string errorString;
        switch (error) {
        case GL_INVALID_ENUM:                  errorString = "INVALID_ENUM"; break;
        case GL_INVALID_VALUE:                 errorString = "INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:             errorString = "INVALID_OPERATION"; break;
        case GL_STACK_OVERFLOW:                errorString = "STACK_OVERFLOW"; break;
        case GL_STACK_UNDERFLOW:               errorString = "STACK_UNDERFLOW"; break;
        case GL_OUT_OF_MEMORY:                 errorString = "OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: errorString = "INVALID_FRAMEBUFFER_OPERATION"; break;
        default:                               errorString = "UNKNOWN_ERROR"; break;
        }
        std::cout << "OpenGL error " << errorString << " (" << error << ") at " << location << std::endl;
    }
}
void reviewport(FCT::Context* ctx,FCT::Window* wnd) {
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
int main() {
    FCT::Runtime* rt = FCT::CreateRuntime();
    rt->init();
    rt->setOpenGLVesion(3, 3);
    auto wnd = rt->createWindow(800, 600, "Triangle Example");
    auto ctx = rt->createContext(wnd);
    FCT::VertexFactory* factory = new FCT::VertexFactory;
    factory->addAttribute(FCT::PipelineAttributeType::Position2f, "aPos");
    factory->addAttribute(FCT::PipelineAttributeType::Color4f, "aColor");
    FCT::Pipeline* pipeline = new FCT::Pipeline(ctx, factory);
    FCT::Rectangle* rectangle = new FCT::Rectangle(ctx,factory);
    rectangle->size(FCT::Vec2(2, 2));
    rectangle->color(FCT::Vec4(0.2f, 0.3f, 0.3f, 1.0f));
    rectangle->create();
    FCT::Triangle* triangle = new FCT::Triangle(ctx,factory);
    triangle->vertex(0,FCT::Vec2(-0.5f, -0.5f));
    triangle->vertex(1,FCT::Vec2(0.5f, -0.5f));
    triangle->vertex(2,FCT::Vec2(0.0f, 0.5f));
    triangle->color(0, FCT::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
    triangle->color(1, FCT::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
    triangle->color(2, FCT::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
    triangle->create();
    ctx->viewport(0, 0, 800, 600);
    bool needViewPort = false;
    wnd->getCallBack()->addResizeCallback([&needViewPort](FCT::Window* wnd, int w, int h) {
        needViewPort = true;
        });
    FCT::Mat4 nullMat;
    FCT::Mat4 rotateMat;
    auto nullMatTex = ctx->createTexture();
    nullMatTex->setSlot(0);
	nullMatTex->create(4, 1,FCT::Texture::Format::RGBA32F);
    nullMatTex->setData(&nullMat, sizeof(nullMat));
    auto rotateMatTex = ctx->createTexture();
    rotateMatTex->setSlot(0);
    rotateMatTex->create(4, 1, FCT::Texture::Format::RGBA32F);
    rotateMatTex->setData(&rotateMat, sizeof(rotateMat));
    while (wnd->isRunning()) {
        if (needViewPort) {
            reviewport(ctx, wnd);
        }
        rotateMat.rotateZ(0.1);
        rotateMatTex->setData(&rotateMat, sizeof(rotateMat));
        ctx->clear(0, 0, 0);
        pipeline->begin();
        nullMatTex->bind();
        pipeline->draw(rectangle);
        rotateMatTex->bind();
        pipeline->draw(triangle);
        pipeline->end();
        wnd->swapBuffers();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

    }
    rectangle->release();
    triangle->release();
	delete pipeline;
    factory->release();
    ctx->release();
    rt->tern();
    return 0;
}
