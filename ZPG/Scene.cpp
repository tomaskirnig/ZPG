#include "Scene.h"

void Scene::addObject(DrawableObject* object) {
    objects.push_back(object);
}

void Scene::deleteObject(DrawableObject* object) {
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i] == object) {
			objects.erase(objects.begin() + i);
			break;
		}
	}
}

void Scene::render() {
    for (DrawableObject* object : objects) {
        object->draw();
    }
}
