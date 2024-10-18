#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Observer.h"

using namespace std;

class Camera {
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Target;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    float Yaw;
    float Pitch;
    float MovementSpeed;
    float MouseSensitivity;
    float Fov;

    // List of observers (shaders)
    vector<Observer*> observers;

    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);

    // Register an observer
    void registerObserver(Observer* observer) {
        observers.push_back(observer);
    }

    // Notify all observers of a change
    void notifyObservers(float aspectRatio);

    // Returns the view matrix calculated using LookAt Matrix
    glm::mat4 GetViewMatrix();

    // Processes input for camera movement
    void ProcessKeyboardMovement(const char direction, float aspectRatio);

    // Processes mouse movement
    void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch, float aspectRatio);

    // Fov control
    void ProcessMouseScroll(float yOffset, float aspectRatio);

private:
    // Updates the camera vectors
    void updateCameraVectors();
};
