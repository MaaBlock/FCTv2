#pragma once
#include "../FCT/headers.h"
using namespace FCT;
#include "World.h"
struct Camera
{
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
    std::vector<Pipeline*> pipelines;
    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;

    Camera(FCT::Vec3 pos = FCT::Vec3(0.0f, 0.0f, 5.0f), float fov = 45.0f,
        float aspectRatio = 4.0f / 3.0f,
        float nearPlane = 0.1f,
        float farPlane = 100.0)
        : position(pos), front(FCT::Vec3(0.0f, 0.0f, -1.0f)), up(FCT::Vec3(0.0f, 1.0f, 0.0f)),
        right(FCT::Vec3(1.0f, 0.0f, 0.0f)), yaw(-90.0f), pitch(0.0f), speed(2.5f), sensitivity(0.1f),
        jumpVelocity(0.0f), gravity(9.8f), isJumping(false),
        fov(fov), aspectRatio(aspectRatio), nearPlane(nearPlane), farPlane(farPlane)

    {
        updateCameraVectors();
        updateProjectionMatrix();
    }
    static constexpr float GROUND_TOLERANCE = 0.01f;
    void updateProjectionMatrix()
    {
        for (auto pipeline : pipelines)
        {
            pipeline->setPerspective(fov, aspectRatio, nearPlane, farPlane);
        }
    }
    void processKeyboard(char* keyState, float deltaTime, const World& world)
    {
        float velocity = speed * deltaTime;
        FCT::Vec3 frontFlat = FCT::normalize(FCT::Vec3(front.x, 0.0f, front.z));
        FCT::Vec3 rightFlat = FCT::normalize(FCT::cross(frontFlat, FCT::Vec3(0.0f, 1.0f, 0.0f)));

        FCT::Vec3 movement(0.0f, 0.0f, 0.0f);

        if (keyState['W'])
            movement += frontFlat;
        if (keyState['S'])
            movement -= frontFlat;
        if (keyState['A'])
            movement -= rightFlat;
        if (keyState['D'])
            movement += rightFlat;

        if (movement.x != 0.0f || movement.z != 0.0f)
        {
            movement = FCT::normalize(movement);
            Vec3 newPosition = position + movement * velocity;

            if (!world.isBlockAt(Vec3(newPosition.x, position.y, position.z)) &&
                !world.isBlockAt(Vec3(newPosition.x, position.y - 1.6f, position.z)))
            {
                position.x = newPosition.x;
            }
            if (!world.isBlockAt(Vec3(position.x, position.y, newPosition.z)) &&
                !world.isBlockAt(Vec3(position.x, position.y - 1.6f, newPosition.z)))
            {
                position.z = newPosition.z;
            }
        }

        if (keyState[FCT::KC_SPACE] && !isJumping)
        {
            jumpVelocity = 5.0f;
            isJumping = true;
        }

        if (isJumping)
        {
            Vec3 newPosition = position;
            newPosition.y += jumpVelocity * deltaTime;
            jumpVelocity -= gravity * deltaTime;

            if (!world.isBlockAt(newPosition + 0.5) && !world.isBlockAt(Vec3(newPosition.x, newPosition.y - 1.6f, newPosition.z)))
            {
                position = newPosition;
            }
            else
            {
                isJumping = false;
                jumpVelocity = 0.0f;
            }
        }

        if (world.isBlockAt(Vec3(position.x, position.y - 1.6f, position.z)))
        {
            isJumping = false;
            jumpVelocity = 0.0f;
            position.y = std::ceil(position.y - 1.6f) + 1.6f;
        }
        else if (!isJumping)
        {
            Vec3 newPosition = position;
            newPosition.y -= gravity * deltaTime;
            if (!world.isBlockAt(Vec3(newPosition.x, newPosition.y - 1.6f - GROUND_TOLERANCE, newPosition.z)))
            {
                position = newPosition;
            }
            else
            {
            }
        }
    }

    void processMouseMovement(float xoffset, float yoffset)
    {
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        updateCameraVectors();
    }

    void updateCameraVectors()
    {
        FCT::Vec3 newFront;
        newFront.x = cos(yaw * 3.14159f / 180.0f) * cos(pitch * 3.14159f / 180.0f);
        newFront.y = sin(pitch * 3.14159f / 180.0f);
        newFront.z = sin(yaw * 3.14159f / 180.0f) * cos(pitch * 3.14159f / 180.0f);
        front = FCT::normalize(newFront);
        right = FCT::normalize(FCT::cross(front, FCT::Vec3(0.0f, 1.0f, 0.0f)));
        up = FCT::normalize(FCT::cross(right, front));
    }
    void updata(Pipeline* pipeline)
    {
        pipeline->lookAt(position, position + front, up);
    }
    void addPipeline(Pipeline* pipeline) {
		pipelines.push_back(pipeline);
        pipeline->setPerspective(fov, aspectRatio, nearPlane, farPlane);
    }
	void removePipeline(Pipeline* pipeline) {
		pipelines.erase(std::remove(pipelines.begin(), pipelines.end(), pipeline), pipelines.end());
	}
    void updata() {
		for (auto pipeline : pipelines) {
            updata(pipeline);
		}
	}
    Vec3 getRayDirection() const
    {
        return front;
    }
};

class Player {
public:
    Player(Camera* camera) {
		m_camera = camera;
    }

private:
    Vec3 m_pos;
    Camera* m_camera;
};
/*
class Camera
{
private:
    Vec3 m_position;
    Vec3 m_target;
    Vec3 m_up;
    float m_fov;
    float m_aspectRatio;
    float m_nearPlane;
    float m_farPlane;

    std::vector<Pipeline *> m_pipelines;

public:
    Camera(const Vec3 &position = Vec3(0.0f, 0.0f, 3.0f),
           const Vec3 &target = Vec3(0.0f, 0.0f, 0.0f),
           const Vec3 &up = Vec3(0.0f, 1.0f, 0.0f),
           float fov = 45.0f,
           float aspectRatio = 16.0f / 9.0f,
           float nearPlane = 0.1f,
           float farPlane = 100.0f);

    void setPosition(const Vec3 &position);
    void setTarget(const Vec3 &target);
    void setUp(const Vec3 &up);
    void setFOV(float fov);
    void setAspectRatio(float aspectRatio);
    void setNearPlane(float nearPlane);
    void setFarPlane(float farPlane);

    void move(const Vec3 &offset);
    void rotate(float yaw, float pitch);

    Vec3 getPosition() const;
    Vec3 getTarget() const;
    Vec3 getUp() const;
    Vec3 getFront() const;

    void update();
    void addPipeline(Pipeline *pipeline);
    void removePipeline(Pipeline *pipeline);

private:
    void updateViewMatrix();
    void updateProjectionMatrix();
};

*/