#include "Camera.h"

// Constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
    Position(position), WorldUp(up), Yaw(yaw), Pitch(pitch), MovementSpeed(0.03f), MouseSensitivity(0.1f), Fov(75.0f) {
    updateCameraVectors();
}

// Returns the view matrix calculated using LookAt matrix
glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(Position, Position + Target, Up);
}

// Notify all observers of a change
void Camera::notifyObservers(float aspectRatio, vector<Light>& lights) {
    glm::mat4 viewMatrix = getViewMatrix();
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(Fov), aspectRatio, 0.1f, 100.0f);
    
    for (IObserver* observer : observers) {
        observer->update(viewMatrix, projectionMatrix, lights[0].getPosition(), lights[0].getColor(), Position);
    }
}

// Move camera in a direction
void Camera::processKeyboardMovement(const char direction, float aspectRatio, vector<Light> lights) {
    float velocity = MovementSpeed;
    if (direction == 'u')
        Position += Target * velocity;  // Move forward
    if (direction == 'd')
        Position -= Target * velocity;  // Move backward
    if (direction == 'l')
        Position -= Right * velocity;   // Move left
    if (direction == 'r')
        Position += Right * velocity;   // Move right

	notifyObservers(aspectRatio, lights);
}

// Change the looking direction of the camera
void Camera::processMouseMovement(float xOffset, float yOffset, float aspectRatio, vector<Light> lights) {
    xOffset *= MouseSensitivity;
    yOffset *= MouseSensitivity;

    Yaw += xOffset;
    Pitch += yOffset;

	// Restrict the camera from flipping over
    if (Pitch > 89.0f) Pitch = 89.0f; 
    if (Pitch < -89.0f) Pitch = -89.0f;
        

    // Update Target, Right, and Up Vectors
    updateCameraVectors();
    notifyObservers(aspectRatio, lights);
}

// Processes input from the mouse scroll wheel
void Camera::processMouseScroll(float yOffset, float aspectRatio, vector<Light> lights) {
    if (Fov >= 1.0f && Fov <= 175.0f)
        Fov -= yOffset;
    if (Fov <= 1.0f)
        Fov = 1.0f;
    if (Fov >= 175.0f)
        Fov = 175.0f;

	std::cout << "Fov: " << Fov << endl;

    notifyObservers(aspectRatio, lights);
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
