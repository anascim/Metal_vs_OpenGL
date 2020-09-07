
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

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), float yaw = YAW, float pitch = PITCH, float fov = FOV, float speed = SPEED);
    void ProcessMovement(int horizontal, int vertical, float deltaTime);
    void ProcessRotation(float xOffset, float yOffset, GLboolean constrainPitch = true);
    void ProcessScroll(float yOffset);
    glm::mat4 GetViewMatrix();

private:
    void updateVectors();
};

#endif // CAMERA_H
