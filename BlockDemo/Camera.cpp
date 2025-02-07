#include "Camera.h"
/*
Camera::Camera(const Vec3 &position, const Vec3 &target, const Vec3 &up,
               float fov, float aspectRatio, float nearPlane, float farPlane)
    : m_position(position), m_target(target), m_up(up),
      m_fov(fov), m_aspectRatio(aspectRatio), m_nearPlane(nearPlane), m_farPlane(farPlane)
{

}

void Camera::setPosition(const Vec3 &position)
{
    m_position = position;
    updateViewMatrix();
}

void Camera::setTarget(const Vec3 &target)
{
    m_target = target;
    updateViewMatrix();
}

void Camera::setUp(const Vec3 &up)
{
    m_up = up;
    updateViewMatrix();
}

void Camera::setFOV(float fov)
{
    m_fov = fov;
    updateProjectionMatrix();
}

void Camera::setAspectRatio(float aspectRatio)
{
    m_aspectRatio = aspectRatio;
    updateProjectionMatrix();
}

void Camera::setNearPlane(float nearPlane)
{
    m_nearPlane = nearPlane;
    updateProjectionMatrix();
}

void Camera::setFarPlane(float farPlane)
{
    m_farPlane = farPlane;
    updateProjectionMatrix();
}
void Camera::move(const Vec3 &offset)
{
    m_position += offset;
    m_target += offset;
    updateViewMatrix();
}

void Camera::rotate(float yaw, float pitch)
{
    Vec3 front = getFront();

    front = front.rotate(m_up, yaw);

    Vec3 right = front.cross(m_up).normalize();
    front = front.rotate(right, pitch);

    m_target = m_position + front;

    m_up = right.cross(front).normalize();

    updateViewMatrix();
}

Vec3 Camera::getPosition() const { return m_position; }
Vec3 Camera::getTarget() const { return m_target; }
Vec3 Camera::getUp() const { return m_up; }

Vec3 Camera::getFront() const
{
    return (m_target - m_position).normalize();
}

void Camera::update()
{
    updateViewMatrix();
    updateProjectionMatrix();
}

void Camera::updateViewMatrix()
{
    for (auto pipeline : m_pipelines) {
        pipeline->lookAt(m_position, m_target, m_up);
    }
}

void Camera::updateProjectionMatrix()
{
    for (auto pipeline : m_pipelines) {
        pipeline->setPerspective(m_fov, m_aspectRatio, m_nearPlane, m_farPlane);
    }
}

void Camera::addPipeline(Pipeline* pipeline) {
    if (pipeline && std::find(m_pipelines.begin(), m_pipelines.end(), pipeline) == m_pipelines.end()) {
        m_pipelines.push_back(pipeline);
        updateViewMatrix();
        updateProjectionMatrix();
    }
}

void Camera::removePipeline(Pipeline* pipeline) {
    auto it = std::find(m_pipelines.begin(), m_pipelines.end(), pipeline);
    if (it != m_pipelines.end()) {
        m_pipelines.erase(it);
    }
}
*/