#pragma once


#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Observer.h"

using namespace std;

class Camera {
private:
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

    // Updates the camera vectors
    void updateCameraVectors();

public:
    // List of observers (shaders)
    vector<Observer*> observers;

    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);

    // Register an observer
    void registerObserver(Observer* observer) {
        observers.push_back(observer);
    }

    void notifyObservers(float aspectRatio);

    glm::mat4 GetViewMatrix();

    void ProcessKeyboardMovement(const char direction, float aspectRatio);

    void ProcessMouseMovement(float xOffset, float yOffset, float aspectRatio);

    void ProcessMouseScroll(float yOffset, float aspectRatio);


};
