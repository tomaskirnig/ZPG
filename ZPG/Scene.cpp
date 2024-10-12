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

int Scene::objectsCount() {
	return objects.size();
}

void Scene::moveObject(int currentObject, char direction) {
    Transformation* transform = objects[currentObject]->getTransformation();
    glm::vec3 currentPosition = transform->getPosition();  // Get the current position

    glm::vec3 movement(0.0f);
    float movementSpeed = 0.01f;

    switch (direction)
    {
        case 'u':
            movement = glm::vec3(0.0f, movementSpeed, 0.0f); // Move along Y-axis
            break;

        case 'd':
            movement = glm::vec3(0.0f, -movementSpeed, 0.0f);
            break;

        case 'l':
            movement = glm::vec3(-movementSpeed, 0.0f, 0.0f); // Move along X-axis
            break;

        case 'r':
            movement = glm::vec3(movementSpeed, 0.0f, 0.0f);
            break;

        default:
            break;
    }

    // Transform movement vector by the rotation matrix to move in local space
    glm::vec3 rotatedMovement = glm::vec3(transform->getRotationMatrix() * glm::vec4(movement, 0.0f));

    transform->setPosition(currentPosition + rotatedMovement);

    // Update the object's transformation matrix
    //objects[currentObject]->applyTransformation(transform);
}


void Scene::rotateObject(int currentObject, int direction) {
    Transformation* transform = objects[currentObject]->getTransformation();

    glm::mat4 rotationMatrix(1.0f);  // Identity matrix for rotation

    // Determine the rotation direction and axis
    switch (direction) {
    case 1:
        rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));  // Rotate +1 degree on X-axis
        break;

    case 2:
        rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), glm::vec3(1.0f, 0.0f, 0.0f));  // Rotate -1 degree on X-axis
        break;

    case 3:
        rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Rotate +1 degree on Y-axis
        break;

    case 4:
        rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Rotate -1 degree on Y-axis
        break;

    case 5:
        rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));  // Rotate +1 degree on Z-axis
        break;

    case 6:
        rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), glm::vec3(0.0f, 0.0f, 1.0f));  // Rotate -1 degree on Z-axis
        break;

    default:
        break;
    }

    // Apply the new rotation matrix to the current object's transformation
    transform->applyRotationMatrix(rotationMatrix);
}


void Scene::scaleObject(int currentObject, char direction) {
    Transformation* transform = objects[currentObject]->getTransformation();

}

void Scene::resetObjectRotation(int currentObject) {
    Transformation* transform = objects[currentObject]->getTransformation();
    transform->resetRotation();
}