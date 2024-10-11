#pragma once
#include <vector>
#include "DrawableObject.h"

class Scene {
    private:
        vector<DrawableObject*> objects;

    public:
        void addObject(DrawableObject* object);
	    void deleteObject(DrawableObject* object);
        void render();
};
