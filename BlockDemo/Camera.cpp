#include "Camera.h"

Camera::Camera(const FCT::Vec3& position, const FCT::Vec3& front, const FCT::Vec3& up)
    : m_position(position), m_front(front), m_up(up), m_yaw(-90.0f), m_pitch(0.0f) {
    updateCameraVectors();
}

void Camera::setPosition(const FCT::Vec3& position) {
    m_position = position;
}

void Camera::setFront(const FCT::Vec3& front) {
    m_front = FCT::normalize(front);
    updateCameraVectors();
}

void Camera::setUp(const FCT::Vec3& up) {
    m_up = FCT::normalize(up);
    updateCameraVectors();
}

void Camera::move(const FCT::Vec3& offset) {
    m_position += offset;
}

void Camera::rotate(float yaw, float pitch) {
    m_yaw += yaw;
    m_pitch += pitch;

    if (m_pitch > 89.0f)
        m_pitch = 89.0f;
    if (m_pitch < -89.0f)
        m_pitch = -89.0f;

    updateCameraVectors();
}

FCT::Mat4 Camera::getViewMatrix() const {
    return FCT::Mat4::lookAt(m_position, m_position + m_front, m_up);
}

FCT::Mat4 Camera::getProjectionMatrix() const {
    return m_projectionMatrix;
}

void Camera::setPerspective(float fov, float aspect, float near, float far) {
    m_projectionMatrix = FCT::Mat4::perspective(fov, aspect, near, far);
}

void Camera::updateCameraVectors() {
    FCT::Vec3 front;
    front.x = cos(FCT::radians(m_yaw)) * cos(FCT::radians(m_pitch));
    front.y = sin(FCT::radians(m_pitch));
    front.z = sin(FCT::radians(m_yaw)) * cos(FCT::radians(m_pitch));
    m_front = FCT::normalize(front);

    m_right = FCT::normalize(FCT::cross(m_front, FCT::Vec3(0.0f, 1.0f, 0.0f)));
    m_up = FCT::normalize(FCT::cross(m_right, m_front));
}