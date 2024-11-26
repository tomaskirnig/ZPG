#include "Camera.h"
#include "SpotLight.h"
#include "DirectionalLight.h"
#include "PointLight.h"

// Constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
    Position(position), WorldUp(up), Yaw(yaw), Pitch(pitch), MovementSpeed(0.03f), MouseSensitivity(0.1f), Fov(75.0f) {
    updateCameraVectors();
}

// Returns the view matrix calculated using LookAt matrix
glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(Position, Position + Target, Up);
}

glm::mat4 Camera::getProjectionMatrix(float aspectRatio) {
	return glm::perspective(glm::radians(Fov), aspectRatio, 0.1f, 100.0f);
}

// Notify all observers of a change
void Camera::notifyObservers(float aspectRatio, vector<Light*> lights) {
    glm::mat4 viewMatrix = getViewMatrix();
	glm::mat4 projectionMatrix = getProjectionMatrix(aspectRatio);
    
    // Convert to LightData
    std::vector<LightData> lightDataList;

    for (auto* light : lights) {
        if (!light) {
            std::cerr << "Warning: Null light pointer found!" << std::endl;
            continue;
        }
        LightData data;

		//cout << "Light Type: " << static_cast<int>(light->getType()) << endl;

		if (light->getType() == LightType::DIRECTIONAL) {
			data.direction = ((DirectionalLight*)light)->getDirection();
		}
		else if (light->getType() == LightType::POINT) {
			data.position = light->getPosition();
		}
		else if (light->getType() == LightType::SPOTLIGHT) {
			data.position = light->getPosition();
			data.direction = ((SpotLight*)light)->getDirection();
			data.cutOff = ((SpotLight*)light)->getCutOff();
			data.outerCutOff = ((SpotLight*)light)->getOuterCutOff();
		}
        
		data.type = light->getType();
        data.color = light->getColor();
        data.intensity = light->getIntensity();

        lightDataList.push_back(data);
    }

    for (IObserver* observer : observers) {
        if (observer) { 
            observer->update(viewMatrix, projectionMatrix, lightDataList, Position);
        }
        else {
            std::cerr << "Warning: Null observer found!" << std::endl;
        }
    }
}

// Move camera in a direction
void Camera::processKeyboardMovement(const char direction, float aspectRatio, vector<Light*> lights) {
    float velocity = MovementSpeed;
    if (direction == 'u')
        Position += Target * velocity;  // Move forward
    if (direction == 'd')
        Position -= Target * velocity;  // Move backward
    if (direction == 'l')
        Position -= Right * velocity;   // Move left
    if (direction == 'r')
        Position += Right * velocity;   // Move right

    if (followingSpotlight) {
        followingSpotlight->setPosition(Position);
    }

	notifyObservers(aspectRatio, lights);
}

// Change the looking direction of the camera
void Camera::processMouseMovement(float xOffset, float yOffset, float aspectRatio, vector<Light*> lights) {
    xOffset *= MouseSensitivity;
    yOffset *= MouseSensitivity;

    Yaw += xOffset;
    Pitch += yOffset;

	// Restrict the camera from flipping over
    if (Pitch > 89.0f) Pitch = 89.0f; 
    if (Pitch < -89.0f) Pitch = -89.0f;
        
    if (followingSpotlight) {
        followingSpotlight->setDirection(Target);
    }

    // Update Target, Right, and Up Vectors
    updateCameraVectors();
    //notifyObservers(aspectRatio, lights);
}

// Processes input from the mouse scroll wheel
void Camera::processMouseScroll(float yOffset, float aspectRatio, vector<Light*> lights) {
    if (Fov >= 1.0f && Fov <= 175.0f)
        Fov -= yOffset;
    if (Fov <= 1.0f)
        Fov = 1.0f;
    if (Fov >= 175.0f)
        Fov = 175.0f;

	std::cout << "Fov: " << Fov << endl;

    notifyObservers(aspectRatio, lights);
}

void Camera::setFollowingSpotlight(Light* spotlight) {
    followingSpotlight = spotlight;
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

void Camera::setSkyBox(DrawableObject* skybox) {
    skyBox = skybox;
}

void Camera::toggleSkyBox() {
    followingSkybox = !followingSkybox;
}

void Camera::drawSkyBox(float aspectRatio)
{
    if (skyBox != nullptr) skyBox->draw(followingSkybox, getViewMatrix(), getProjectionMatrix(aspectRatio));
	else std::cerr << "Warning: Skybox not set!" << std::endl;
}
