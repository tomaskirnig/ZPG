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

		// Object controls
		void moveObject(int currentObject, char direction); // u - up, d - down, l - left, r - right
		void rotateObject(int currentObject, int direction); // 1 - +x, 2 - -x, 3 - +y, 4 - -y, 5 - +z, 6 - -z
		void resetObjectRotation(int currentObject);
		void scaleObject(int currentObject, char direction); // u - up, d - down
		void resetObjectScale(int currentObject);
};
