#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = -90.0f, float pitch = 0.0f);

    glm::mat4 GetViewMatrix() const;
    void ProcessKeyboard(char direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    glm::vec3 GetPosition() const { return Position; }
    void SetPosition(glm::vec3 position) { Position = position; }

private:
    void updateCameraVectors();

    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    float Yaw;
    float Pitch;
    float MovementSpeed = 50.0f;
    float MouseSensitivity = 0.1f;
};
