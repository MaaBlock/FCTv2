#include "../FCT/headers.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <thread>
#include <iostream>
#include <cmath>
#include <map>
using namespace FCT;

namespace std {
    template <>
    struct hash<FCT::Vec3> {
        size_t operator()(const FCT::Vec3& v) const {
            size_t h1 = std::hash<float>{}(v.x);
            size_t h2 = std::hash<float>{}(v.y);
            size_t h3 = std::hash<float>{}(v.z);

            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
}
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

class Block {
public:
    Box* box;
    static Context* ctx;
	static VertexFactory* factory;
    static ImageLoader* il;
	static Texture* texture;
    
    static void Init(Context* ctx, VertexFactory* factory, ImageLoader* il) {
		Block::ctx = ctx;
		Block::factory = factory;
		Block::il = il;
		texture = ctx->createTexture();
		texture->setSlot(3);
        if (texture->loadFromFile("MaBlock.png", il)) {
			std::cout << "Texture loaded successfully" << std::endl;
        }
	}
    Block() {
		box = new Box(ctx, factory);
        box->texture(texture);
		box->size(FCT::Vec3(1, 1, 1));
		box->setTextureCoordinates();
        box->color(Vec4(1, 1, 1, 1));
		box->create();
	}
    ~Block() {
        box->release();
    }
    void setPos(Vec3 pos) {
        box->setPosition(pos);
    }
	void render(Pipeline* pipeline) {
        pipeline->draw(box);
	}

};
Context* Block::ctx = nullptr;
VertexFactory* Block::factory = nullptr;
ImageLoader* Block::il = nullptr;
Texture* Block::texture = nullptr;

class World {
public:
    std::unordered_map<Vec3, Block*> blocks;
    Block* selectedBlock = nullptr;
    Vec3 selectedPos;
    World() {
		for (int x = -64; x <= 64; ++x) {
			for (int z = -64; z <= 64; ++z) {
				addBlock(Vec3(x, 0, z));
			}
		}
	}
    void addBlock(Vec3 pos) {
        blocks[pos] = new Block;
        blocks[pos]->setPos(pos);
    }

    void removeBlock(Vec3 pos) {
        auto it = blocks.find(pos);
        if (it != blocks.end()) {
            delete it->second;
            blocks.erase(it);
        }
    }
    bool isBlockAt(const Vec3& pos) const {
        return blocks.find(Vec3(std::round(pos.x), std::round(pos.y), std::round(pos.z))) != blocks.end();
    }
    Block* getBlock(Vec3 pos) {
        auto it = blocks.find(pos);
        if (it != blocks.end()) {
            return it->second;
        }
        return nullptr;
    }

    void render(Pipeline* pipeline, const Vec3& playerPosition) {
        for (const auto& pair : blocks) {
            const Vec3& blockPos = pair.first;
            Block* block = pair.second;

            Vec3 diff = blockPos - playerPosition;
            float distanceSquared = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;

            if (distanceSquared <= 16 * 16) {
                block->render(pipeline);
            }
        }
    }

    void selectBlock(const Vec3& pos) {
        auto it = blocks.find(pos);
        if (it != blocks.end()) {
            if (selectedBlock) {
                selectedBlock->box->color(Vec4(1, 1, 1, 1));
                selectedBlock->box->updata();
            }
            selectedBlock = it->second;
            selectedBlock->box->color(Vec4(1.2, 1.2, 1.2, 1));
            selectedBlock->box->updata();
            selectedPos = pos;
        }
    }
    void destroyBlock(const Vec3& pos) {
        removeBlock(pos);
        if (selectedBlock) {
            if (pos == selectedPos) {
                selectedBlock = nullptr;
            }
        }
    }

    void placeBlock(const Vec3& pos, const Vec3& normal) {
        Vec3 placePos = pos + normal;
        if (!isBlockAt(placePos)) {
            addBlock(placePos);
        }
    }

    std::pair<bool, Vec3> raycast(const Vec3& start, const Vec3& direction, float maxDistance) {
        Vec3 step = direction * 0.1f;
        Vec3 current = start;
        float distance = 0.0f;

        while (distance < maxDistance) {
            Vec3 checkPos = Vec3(std::round(current.x), std::round(current.y), std::round(current.z));
            if (isBlockAt(checkPos)) {
                return { true, checkPos };
            }
            current += step;
            distance += 0.1f;
        }

        return { false, current };
    }

    Vec3 getNormal(const Vec3& hitPos, const Vec3& rayStart) {
        Vec3 diff = hitPos - rayStart;
        float absX = std::abs(diff.x);
        float absY = std::abs(diff.y);
        float absZ = std::abs(diff.z);

        if (absX > absY && absX > absZ) {
            return Vec3(diff.x > 0 ? -1 : 1, 0, 0);
        }
        else if (absY > absZ) {
            return Vec3(0, diff.y > 0 ? -1 : 1, 0);
        }
        else {
            return Vec3(0, 0, diff.z > 0 ? -1 : 1);
        }
    }
};

class Camera {
public:
    Vec3 position;
    Vec3 front;
    Vec3 up;
    Vec3 right;
    float yaw;
    float pitch;
    float speed;
    float sensitivity;
    float jumpVelocity;
    float gravity;
    bool isJumping;

    Camera(FCT::Vec3 pos = FCT::Vec3(0.0f, 0.0f, 5.0f))
        : position(pos), front(FCT::Vec3(0.0f, 0.0f, -1.0f)), up(FCT::Vec3(0.0f, 1.0f, 0.0f)),
        right(FCT::Vec3(1.0f, 0.0f, 0.0f)), yaw(-90.0f), pitch(0.0f), speed(2.5f), sensitivity(0.1f),
        jumpVelocity(0.0f), gravity(9.8f), isJumping(false) {
        updateCameraVectors();
    }
    static constexpr float GROUND_TOLERANCE = 0.01f;


    void processKeyboard(char* keyState, float deltaTime, const World& world) {
        float velocity = speed * deltaTime;
        FCT::Vec3 frontFlat = FCT::normalize(FCT::Vec3(front.x, 0.0f, front.z));
        FCT::Vec3 rightFlat = FCT::normalize(FCT::cross(frontFlat, FCT::Vec3(0.0f, 1.0f, 0.0f)));

        FCT::Vec3 movement(0.0f, 0.0f, 0.0f);

        if (keyState['W']) movement += frontFlat;
        if (keyState['S']) movement -= frontFlat;
        if (keyState['A']) movement -= rightFlat;
        if (keyState['D']) movement += rightFlat;

        if (movement.x != 0.0f || movement.z != 0.0f) {
            movement = FCT::normalize(movement);
            Vec3 newPosition = position + movement * velocity;

            if (!world.isBlockAt(Vec3(newPosition.x, position.y, position.z)) &&
                !world.isBlockAt(Vec3(newPosition.x, position.y - 1.6f, position.z))) {
                position.x = newPosition.x;
            }
            if (!world.isBlockAt(Vec3(position.x, position.y, newPosition.z)) &&
                !world.isBlockAt(Vec3(position.x, position.y - 1.6f, newPosition.z))) {
                position.z = newPosition.z;
            }
        }

        if (keyState[FCT::KC_SPACE] && !isJumping) {
            jumpVelocity = 5.0f;
            isJumping = true;
        }

        if (isJumping) {
            Vec3 newPosition = position;
            newPosition.y += jumpVelocity * deltaTime;
            jumpVelocity -= gravity * deltaTime;

            if (!world.isBlockAt(newPosition + 0.5) && !world.isBlockAt(Vec3(newPosition.x, newPosition.y - 1.6f, newPosition.z))) {
                position = newPosition;
            }
            else {
                isJumping = false;
                jumpVelocity = 0.0f;
            }
        }

        if (world.isBlockAt(Vec3(position.x, position.y - 1.6f, position.z))) {
            isJumping = false;
            jumpVelocity = 0.0f;
            position.y = std::ceil(position.y - 1.6f) + 1.6f; 
        }
        else if (!isJumping) {
            Vec3 newPosition = position;
            newPosition.y -= gravity * deltaTime;
            if (!world.isBlockAt(Vec3(newPosition.x, newPosition.y - 1.6f - GROUND_TOLERANCE, newPosition.z))) {
                position = newPosition;
            }
            else {

            }
        }
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
    void bind(Pipeline* pipeline) {
        pipeline->lookAt(position, position + front, up);
    }
    Vec3 getRayDirection() const {
        return front;
    }
};

void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}
int main() {
    FCT::Runtime* rt = FCT::CreateRuntime();
    rt->init();
    auto font = rt->createFont();
    auto emjFont = rt->createFont();
    rt->setOpenGLVesion(3, 3);
    auto wnd = rt->createWindow(800, 600, "test0");
    auto ctx = rt->createContext(wnd);
    auto il = rt->createImageLoader();
    FCT::VertexFactory* factory = new FCT::VertexFactory;
    factory->addAttribute(FCT::PipelineAttributeType::Position3f, "aPos");
    factory->addAttribute(FCT::PipelineAttributeType::Color4f, "aColor");
    factory->addAttribute(FCT::PipelineAttributeType::TexCoord2f, "aTexCoord");
    FCT::Pipeline* pipeline = new FCT::Pipeline(ctx, factory);
    VertexRenderPipeline* vrp = new VertexRenderPipeline(ctx);
    VertexRenderScreen* screen = new VertexRenderScreen(vrp);
    screen->setPosition(Vec3(0, 0.5 + 2 + 1, -3));
    screen->size(Vec3(2, 2, 0.51));
    screen->viewport(-20, -20, 100, 100);
    screen->create();
    float fov = 45.0f * 3.14159f / 180.0f;
    float aspect = 4.0f / 3.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
    pipeline->setPerspective(fov, aspect, nearPlane, farPlane);
    vrp->setPerspective(fov, aspect, nearPlane, farPlane);
	Block::Init(ctx, factory, il);
    World world;
    //font->create("Noto-COLRv1.ttf");
    //font->create("NotoColorEmoji.ttf");
    //font->create("NotoColorEmoji_WindowsCompatible.ttf");
    //font->create("C://Windows//Fonts//seguiemj.ttf");
    //font->create("Twemoji.Mozilla.ttf");
    emjFont->create("C://Windows//Fonts//seguiemj.ttf");
    font->create("C:\\Windows\\Fonts\\simsun.ttc");
    emjFont->translateGlyph(U'\U0001F600');
    emjFont->translateGlyph(U'🔥');
    emjFont->translateGlyph(U'🥹');
    font->translateGlyph(L'草');
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

    bool leftMousePressed = false;
    bool rightMousePressed = false;
    wnd->getCallBack()->addRButtonUpCallback([&rightMousePressed](Window* wnd,int x,int y) {
        rightMousePressed = true;
        });
    wnd->getCallBack()->addLButtonUpCallback([&leftMousePressed](Window* wnd, int x, int y) {
        leftMousePressed = true;
        });
    Camera camera(FCT::Vec3(0.0f, 0.5f + 1.6f + 3, 0.0f));
    auto lastFrame = std::chrono::high_resolution_clock::now();
    glfwSetErrorCallback(glfw_error_callback);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    while (wnd->isRunning()) {
        auto currentFrame = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentFrame - lastFrame).count();
        lastFrame = currentFrame;
        if (needViewPort) {
            reviewport(ctx, wnd);
            needViewPort = false;
        }
        Vec3 rayDirection = camera.getRayDirection();
        std::pair<bool, Vec3> ret = world.raycast(camera.position, rayDirection, 5.0f);
        auto hit = ret.first;
        auto hitPosition = ret.second;
            

        if (hit) {
            world.selectBlock(hitPosition);
            if (leftMousePressed) {
                world.destroyBlock(hitPosition);
                leftMousePressed = false;
            }
            if (rightMousePressed) {
                Vec3 normal = world.getNormal(hitPosition, camera.position);
                world.placeBlock(hitPosition, normal);
                rightMousePressed = false;
            }
        }

        camera.processKeyboard(KeyState, deltaTime, world);
        camera.processMouseMovement(mouseDx, mouseDy);
        mouseDx = 0;
        mouseDy = 0;
        glEnable(GL_DEPTH_TEST);
        camera.bind(pipeline);
        camera.bind(vrp);
        ctx->clear(0.2f, 0.3f, 0.3f);
        pipeline->begin();
        world.render(pipeline, camera.position);
        pipeline->end();
        vrp->begin(screen);
        //vrp->rectangle(Vec2(15, 0), Vec2(20, 20),1);
        
        //vrp->beginPath(1);
        //vrp->moveTo(Vec2(20, 15));
		//vrp->arcTo(Vec2(10, 15), 0, 2 * 3.14159);
        //vrp->endPath();
        /*
        vrp->beginPath(0);
        vrp->moveTo(Vec2(15, 15));
        vrp->lineTo(Vec2(20, 15));
        vrp->lineTo(Vec2(35, 15));
        vrp->lineTo(Vec2(35, 35));
        vrp->lineTo(Vec2(15, 35));
        vrp->lineTo(Vec2(15, 15));
        vrp->endPath();//*/
		//vrp->drawChar(font, U'\U0001F600', 15, 15);
        Mat4 tmp;
		tmp.scale(0.1, 0.1, 1);
        vrp->setTransform(tmp);
		vrp->drawChar(font, L'草', 15, 15);
        tmp.scale(0.25, 0.25, 1);
        tmp.translate(1, 0);
        vrp->setTransform(tmp);
		vrp->drawChar(emjFont, U'🔥', 15, 15);
        tmp.translate(2, 0);
        vrp->setTransform(tmp);
		vrp->drawChar(emjFont, U'🥹', 15, 15);
        vrp->end(); 
        wnd->swapBuffers();
        GL_Check("loop end");
    }
    delete pipeline;
    factory->release();
    ctx->release();
    rt->tern();
    return 0;
}