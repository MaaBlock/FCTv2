#include "../FCT/headers.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <thread>
#include <iostream>
#include <cmath>

void GL_Check(const char* describe) {
    int error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << describe << ": OpenGL error " << error << std::endl;
    }
}

void reviewport(FCT::Context* ctx, FCT::Window* wnd) {
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
    auto wnd = rt->createWindow(800, 600, "3D Example");
    auto ctx = rt->createContext(wnd);
    FCT::VertexFactory* factory = new FCT::VertexFactory;
    factory->addAttribute(FCT::PipelineAttributeType::Position3f, "aPos");
    factory->addAttribute(FCT::PipelineAttributeType::Color4f, "aColor");
    FCT::Pipeline* pipeline = new FCT::Pipeline(ctx, factory);

    float fov = 45.0f * 3.14159f / 180.0f; 
    float aspect = 4.0f / 3.0f; 
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
    pipeline->setPerspective(fov, aspect, nearPlane, farPlane);

    FCT::Box* box = new FCT::Box(ctx, factory);
    box->size(FCT::Vec3(1, 1, 1));
    box->color(FCT::Vec4(1, 1, 1, 1));
    box->create();

    reviewport(ctx, wnd); 
    bool needViewPort = false;
    wnd->getCallBack()->addResizeCallback([&needViewPort](FCT::Window* wnd, int w, int h) {
        needViewPort = true;
        });

    float angle = 0.0f;
    while (wnd->isRunning()) {
        if (needViewPort) {
            reviewport(ctx, wnd);
            needViewPort = false;
        }

        angle += 0.01f;
        float camX = sin(angle) * 5.0f;
        float camZ = cos(angle) * 5.0f;
        FCT::Vec3 eye(camX, 2.0f, camZ);
        FCT::Vec3 target(0.0f, 0.0f, 0.0f);
        FCT::Vec3 up(0.0f, 1.0f, 0.0f);
        pipeline->lookAt(eye, target, up);

        ctx->clear(0.2f, 0.3f, 0.3f);
        pipeline->begin();
        pipeline->draw(box);
        pipeline->end();
        wnd->swapBuffers();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    box->release();
    delete pipeline;
    factory->release();
    ctx->release();
    rt->tern();
    return 0;
}