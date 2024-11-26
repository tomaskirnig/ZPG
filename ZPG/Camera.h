#pragma once


#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "ISubject.h"
#include "Light.h"

using namespace std;

class Camera : ISubject {
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

    Light* followingSpotlight = nullptr;

	DrawableObject* skyBox = nullptr;
    bool followingSkybox = true;

    // Updates the camera vectors
    void updateCameraVectors();

public:
    // List of observers (shaders)
    vector<IObserver*> observers;

    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);

    // Register an observer
    void registerObserver(IObserver* observer) override {
        observers.push_back(observer);
    }

    void notifyObservers(float aspectRatio, vector<Light*> lights) override;

    glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix(float aspectRatio);

    void processKeyboardMovement(const char direction, float aspectRatio, vector<Light*> lights);

    void processMouseMovement(float xOffset, float yOffset, float aspectRatio, vector<Light*> lights);

    void processMouseScroll(float yOffset, float aspectRatio, vector<Light*> lights);

    glm::vec3 getPosition() const {
        return Position;
    }

	glm::vec3 getTarget() const {
		return Target;
	}

    void setFollowingSpotlight(Light* spotlight);
	Light* getFollowingSpotlight() const { return followingSpotlight; }

    void setSkyBox(DrawableObject* skybox);
	DrawableObject* getSkyBox() const { return skyBox; }
	bool isSetSkyBox() const { return skyBox != nullptr; }
    void toggleSkyBox();
	bool isFollowingSkybox() const { return followingSkybox; }
	void setFollowingSkybox(bool follow) { followingSkybox = follow; }

    void drawSkyBox(float aspectRatio);
};
