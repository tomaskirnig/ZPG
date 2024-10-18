#include "Camera.h"

// Constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
    Position(position), WorldUp(up), Yaw(yaw), Pitch(pitch), MovementSpeed(0.05f), MouseSensitivity(0.1f), Fov(75.0f) {
    updateCameraVectors();
}

// Returns the view matrix calculated using LookAt matrix
glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(Position, Position + Target, Up);
}

void Camera::notifyObservers(float aspectRatio) {
    glm::mat4 viewMatrix = GetViewMatrix();
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(Fov), aspectRatio, 0.1f, 100.0f);
    for (Observer* observer : observers) {
        observer->update(viewMatrix, projectionMatrix);
    }
}

// Processes input from keyboard for moving the camera
void Camera::ProcessKeyboardMovement(const char direction, float aspectRatio) {
    float velocity = MovementSpeed;
    if (direction == 'w')
        Position += Target * velocity;  // Move forward
    if (direction == 's')
        Position -= Target * velocity;  // Move backward
    if (direction == 'a')
        Position -= Right * velocity;   // Move left
    if (direction == 'd')
        Position += Right * velocity;   // Move right

	notifyObservers(aspectRatio);
}

 //Processes input from mouse movement for rotating the camera
void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch, float aspectRatio) {
    xOffset *= MouseSensitivity;
    yOffset *= MouseSensitivity;

    Yaw += xOffset;
    Pitch += yOffset;

    // Make sure that when pitch is out of bounds, the screen doesn't get flipped
    if (constrainPitch) {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // Update Target, Right, and Up Vectors using the updated Euler angles
    updateCameraVectors();
    notifyObservers(aspectRatio);
}

// Processes input from the mouse scroll wheel
void Camera::ProcessMouseScroll(float yOffset, float aspectRatio) {
    if (Fov >= 1.0f && Fov <= 45.0f)
        Fov -= yOffset;
    if (Fov <= 1.0f)
        Fov = 1.0f;
    if (Fov >= 45.0f)
        Fov = 45.0f;

    notifyObservers(aspectRatio);
}

// Updates the Target, Right, and Up vectors using the current Euler angles
void Camera::updateCameraVectors() {
    // Calculate the new target direction
    glm::vec3 target;
    target.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    target.y = sin(glm::radians(Pitch));
    target.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Target = glm::normalize(target);

    // Also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Target, WorldUp));  // Normalize the vectors
    Up = glm::normalize(glm::cross(Right, Target));
}
