#include "pch.h"
#include "Camera.h"

Camera::Camera(const glm::vec3& position)
    : mPosition(position), mYaw(-90.0f), mPitch(0)
{

}

Camera::~Camera()
{
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(mPosition, mPosition + Forward(), Up());
}

glm::vec3 Camera::Forward()
{
    glm::vec3 front;
    front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    front.y = sin(glm::radians(mPitch));
    front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    front = glm::normalize(front);
    return front;
}

glm::vec3 Camera::Right()
{
    return glm::normalize(glm::cross(Forward(), UP));
}

glm::vec3 Camera::Up()
{
    return glm::normalize(glm::cross(Right(), Forward()));
}