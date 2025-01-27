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

class Camera {
public:
    FCT::Vec3 position;
    FCT::Vec3 front;
    FCT::Vec3 up;
    FCT::Vec3 right;
    float yaw;
    float pitch;
    float speed;
    float sensitivity;

    Camera(FCT::Vec3 pos = FCT::Vec3(0.0f, 0.0f, 5.0f))
        : position(pos), front(FCT::Vec3(0.0f, 0.0f, -1.0f)), up(FCT::Vec3(0.0f, 1.0f, 0.0f)),
        right(FCT::Vec3(1.0f, 0.0f, 0.0f)), yaw(-90.0f), pitch(0.0f), speed(0.05f), sensitivity(0.1f) {
        updateCameraVectors();
    }

    void processKeyboard(char* keyState) {
        FCT::Vec3 frontFlat = FCT::normalize(FCT::Vec3(front.x, 0.0f, front.z));
        FCT::Vec3 rightFlat = FCT::normalize(FCT::cross(frontFlat, FCT::Vec3(0.0f, 1.0f, 0.0f)));

        if (keyState['W']) position += frontFlat * speed;
        if (keyState['S']) position -= frontFlat * speed;
        if (keyState['A']) position -= rightFlat * speed;
        if (keyState['D']) position += rightFlat * speed;
        if (keyState[FCT::KC_SPACE]) position += FCT::Vec3(0.0f, 1.0f, 0.0f) * speed;
        if (keyState[FCT::KC_LEFT_SHIFT]) position -= FCT::Vec3(0.0f, 1.0f, 0.0f) * speed;
    }

    void processMouseMovement(float xoffset, float yoffset) {
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        updateCameraVectors();
    }

    void updateCameraVectors() {
        FCT::Vec3 newFront;
        newFront.x = cos(yaw * 3.14159f / 180.0f) * cos(pitch * 3.14159f / 180.0f);
        newFront.y = sin(pitch * 3.14159f / 180.0f);
        newFront.z = sin(yaw * 3.14159f / 180.0f) * cos(pitch * 3.14159f / 180.0f);
        front = FCT::normalize(newFront);
        right = FCT::normalize(FCT::cross(front, FCT::Vec3(0.0f, 1.0f, 0.0f)));
        up = FCT::normalize(FCT::cross(right, front));
    }
};

int main() {
    FCT::Runtime* rt = FCT::CreateRuntime();
    rt->init();
    rt->setOpenGLVesion(3, 3);
    auto wnd = rt->createWindow(800, 600, "3D Example");
    auto ctx = rt->createContext(wnd);
    auto il = rt->createImageLoader();
    FCT::VertexFactory* factory = new FCT::VertexFactory;
    factory->addAttribute(FCT::PipelineAttributeType::Position3f, "aPos");
    factory->addAttribute(FCT::PipelineAttributeType::Color4f, "aColor");
    factory->addAttribute(FCT::PipelineAttributeType::TexCoord2f, "aTexCoord");
    FCT::Pipeline* pipeline = new FCT::Pipeline(ctx, factory);
    float fov = 45.0f * 3.14159f / 180.0f;
    float aspect = 4.0f / 3.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
    pipeline->setPerspective(fov, aspect, nearPlane, farPlane);

    FCT::Box* box = new FCT::Box(ctx, factory);
    box->size(FCT::Vec3(1, 1, 1));
    box->color(FCT::Vec4(1, 1, 1, 1));
    box->setTextureCoordinates();
    box->create();
    FCT::Texture* texture = ctx->createTexture();
    texture->setSlot(3);
    if (texture->loadFromFile("MaBlock.png", il)) {
        std::cout << "Texture loaded successfully." << std::endl;
    };
    reviewport(ctx, wnd);
    bool needViewPort = false;
    wnd->getCallBack()->addResizeCallback([&needViewPort](FCT::Window* wnd, int w, int h) {
        needViewPort = true;
        });
    char KeyState[512] = {};
    wnd->getCallBack()->addKeyDownCallback([&KeyState](FCT::Window* wnd, int key) {
        KeyState[key] = true;
        });
    bool mouseCanMove = false;
    wnd->getCallBack()->addKeyUpCallback([&KeyState,&mouseCanMove](FCT::Window* wnd, int key) {
		if (key == FCT::KC_ESCAPE) 
            mouseCanMove = !mouseCanMove;
        KeyState[key] = false;
        });
    int mouseDx = 0, mouseDy = 0;
    wnd->getCallBack()->addMouseMoveCallback([&](FCT::Window* wnd, int x, int y) {
        if (!mouseCanMove) {
            mouseDx = x - wnd->getWidth() / 2;
            mouseDy = wnd->getHeight() / 2 - y; 
            wnd->setCursorPos(wnd->getWidth() / 2, wnd->getHeight() / 2);
        }
        });

    Camera camera(FCT::Vec3(0.0f, 0.0f, 5.0f));

    while (wnd->isRunning()) {
        if (needViewPort) {
            reviewport(ctx, wnd);
            needViewPort = false;
        }

        camera.processKeyboard(KeyState);
        camera.processMouseMovement(mouseDx, mouseDy);
        mouseDx = 0;
        mouseDy = 0;

        FCT::Vec3 eye = camera.position;
        FCT::Vec3 target = camera.position + camera.front;
        FCT::Vec3 up = camera.up;
        pipeline->lookAt(eye, target, up);

        glEnable(GL_DEPTH_TEST);
        ctx->clear(0.2f, 0.3f, 0.3f);
        pipeline->begin();
        texture->bind();
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