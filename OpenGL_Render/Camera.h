
#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include "glm.hpp"
#include "matrix_transform.hpp"

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float FOV         =  45.0f;
const float MIN_FOV     =  1.0f;
const float MAX_FOV     =  400.0f;

class Camera
{
public:

    // Positioning
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Perspective
    float Fov;
    // Rotation
    float Yaw;
    float Pitch;
    // Control
    float Speed;
    float Sensitivity;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), float yaw = YAW, float pitch = PITCH, float fov = FOV, float speed = SPEED) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), Up(glm::vec3(0.0f, 1.0f, 0.0f)), Speed(SPEED), Sensitivity(SENSITIVITY)
    {
        Position = position;
        WorldUp = Up;
        Fov = fov;
        Yaw = yaw;
        Pitch = pitch;
        Speed = speed;
        updateVectors();
    }


    void ProcessMovement(int horizontal, int vertical, float deltaTime)
    {
        // Input is supposed to be a value from -1 to 1 on both axis
        Position += (float) vertical * Front * deltaTime * Speed;
        Position += (float) horizontal * Right * deltaTime * Speed;
    }

    void ProcessRotation(float xOffset, float yOffset, GLboolean constrainPitch = true)
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

    void ProcessScroll(float yOffset)
    {
        Fov += yOffset;
        if (Fov > MAX_FOV)
            Fov = MAX_FOV;
        else if (Fov < MIN_FOV)
            Fov = MIN_FOV;
    }

    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

private:
    void updateVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

        Front = normalize(front);
        Right = glm::normalize(glm::cross(WorldUp, -Front));
        Up = glm::normalize(glm::cross(Right, Front));
    }
};

#endif // CAMERA_H
