#pragma once
#include "../FCT/headers.h"

class Camera {
public:
    Camera(const FCT::Vec3& position = FCT::Vec3(0.0f, 0.0f, 0.0f),
           const FCT::Vec3& front = FCT::Vec3(0.0f, 0.0f, -1.0f),
           const FCT::Vec3& up = FCT::Vec3(0.0f, 1.0f, 0.0f));

    void setPosition(const FCT::Vec3& position);
    void setFront(const FCT::Vec3& front);
    void setUp(const FCT::Vec3& up);

    void move(const FCT::Vec3& offset);
    void rotate(float yaw, float pitch);

    FCT::Mat4 getViewMatrix() const;
    FCT::Mat4 getProjectionMatrix() const;

    void setPerspective(float fov, float aspect, float near, float far);

    const FCT::Vec3& getPosition() const { return m_position; }
    const FCT::Vec3& getFront() const { return m_front; }
    const FCT::Vec3& getUp() const { return m_up; }

private:
    FCT::Vec3 m_position;
    FCT::Vec3 m_front;
    FCT::Vec3 m_up;
    FCT::Vec3 m_right;
    float m_yaw;
    float m_pitch;

    FCT::Mat4 m_projectionMatrix;

    void updateCameraVectors();
};