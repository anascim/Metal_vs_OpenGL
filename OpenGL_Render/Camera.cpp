#include "Camera.h"
Camera::Camera(glm::vec3 position, float yaw, float pitch, float fov, float speed) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), Up(glm::vec3(0.0f, 1.0f, 0.0f)), Speed(SPEED), Sensitivity(SENSITIVITY)
{
    Position = position;
    WorldUp = Up;
    Fov = fov;
    Yaw = yaw;
    Pitch = pitch;
    Speed = speed;
    updateVectors();
}

void Camera::ProcessMovement(int horizontal, int vertical, float deltaTime)
{
    // Input is supposed to be a value from -1 to 1 on both axis
    Position += (float) vertical * Front * deltaTime * Speed;
    Position += (float) horizontal * Right * deltaTime * Speed;
}

void Camera::ProcessRotation(float xOffset, float yOffset, GLboolean constrainPitch)
{
    Yaw += xOffset * Sensitivity;
    Pitch += yOffset * Sensitivity;

    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        else if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    updateVectors();
}

void Camera::ProcessScroll(float yOffset)
{
    Fov += yOffset;
    if (Fov > MAX_FOV)
        Fov = MAX_FOV;
    else if (Fov < MIN_FOV)
        Fov = MIN_FOV;
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::updateVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

    Front = normalize(front);
    Right = glm::normalize(glm::cross(WorldUp, -Front));
    Up = glm::normalize(glm::cross(Right, Front));
}
