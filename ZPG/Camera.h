#pragma once


#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Subject.h"

using namespace std;

class Camera : Subject {
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
    void registerObserver(Observer* observer) override {
        observers.push_back(observer);
    }

    void notifyObservers(float aspectRatio, vector<Light>& lights) override;

    glm::mat4 GetViewMatrix();

    void ProcessKeyboardMovement(const char direction, float aspectRatio, vector<Light> lights);

    void ProcessMouseMovement(float xOffset, float yOffset, float aspectRatio, vector<Light> lights);

    void ProcessMouseScroll(float yOffset, float aspectRatio, vector<Light> lights);


};
