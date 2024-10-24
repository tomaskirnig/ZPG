#pragma once
#include <vector>
#include "DrawableObject.h"

class Scene {
    private:
        std::vector<DrawableObject*> objects;

    public:
        void addObject(DrawableObject* object);
	    void deleteObject(DrawableObject* object);

        void render();
		int objectsCount();

		vector<Shader*> getShaders();

		// Object controls
		void moveObject(int object, char direction);
		void moveObject(int object, char direction, float amount);
		void rotateObject(int currentObject, int direction);
		void resetObjectRotation(int currentObject);
		void scaleObject(int currentObject, char direction);
		void resetObjectScale(int currentObject);
};
