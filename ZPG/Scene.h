#pragma once
#include <vector>
#include <map>
#include "DrawableObject.h"
#include "Camera.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "DirectionalLight.h"

class Scene {
    private:
        vector<DrawableObject*> objects;
		std::map<std::tuple<Shader*, Model*>, std::vector<DrawableObject*>> groupedObjects;
		vector<Camera*> cameras;
		vector<Light*> lights;

		int currentObject;
		int currentCamera;

		float aspectRatio;

		void groupObjectsForInstancing();

    public:
		Scene();
		~Scene();

        void addObject(DrawableObject* object);
	    void deleteObject(DrawableObject* object);

		void addCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f);
		
		void addLight(std::shared_ptr<Model> model, LightType type);
		void addLight(std::shared_ptr<Model> model, glm::vec3 position, glm::vec3 color, float intensity, LightType type);
		Light* addLight(std::shared_ptr<Model> model, glm::vec3 position, glm::vec3 color, float intensity, LightType type, bool follow);
		void setFollowingSpotLight(std::shared_ptr<Model> model);

        void render();
		void registerAllObservers(float aspectRatio);
		void notifyCurrObservers(float aspectRatio);

		int objectsCount();
		int getCurrCamera();
		int getCurrObject();
		int getNumOfLights();

		void setAspectRatio(float aspectRatio);

		// Object controls
		void currentObjectPlus();
		void currentObjectMinus();

		void currentCameraPlus(float aspectRatio);

		vector<Shader*> getShaders();

		// Object controls
		void moveObject(int object, char direction);
		void moveObject(int object, char direction, float amount);
		void moveLight(int light, char direction);
		void moveLight(int light, char direction, float amount);
		void rotateObject(int currentObject, int direction);
		void resetObjectRotation(int currentObject);
		void scaleObject(int currentObject, char direction);
		void resetObjectScale(int currentObject);

		glm::vec3 getPositionLight(int light);
		glm::vec3 getPositionObject(int object);

		LightType getLightType(int light);

		// Camera controls
		void moveCamera(int camera, char direction, float aspectRatio);
		void mouseMovementCamera(int camera, float xOffset, float yOffset, float aspectRatio);
		void zoomCamera(int camera, double yOffset, float aspectRatio);

};
