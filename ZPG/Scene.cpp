#include "Scene.h"

// Adds a new object to the scene
void Scene::addObject(DrawableObject* object) {
    objects.push_back(object);
}

// Deletes an object from the scene
void Scene::deleteObject(DrawableObject* object) {
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i] == object) {
			objects.erase(objects.begin() + i);
			break;
		}
	}
}

// Renders all objects in the scene
void Scene::render() {
    //// Apply camera's view and projection matrices
    //cameras[currentCamera].notifyObservers(aspectRatio);

    //// Apply light properties to each shader
    //for (DrawableObject* object : objects) {
    //    Shader* shader = object->getShader();
    //    light.applyLighting(shader);  // Apply light to each object shader
    //}

    for (DrawableObject* object : objects) {
        object->draw();
    }
}

// Returns the number of objects in the scene
int Scene::objectsCount() {
	return objects.size();
}

// Moves the current object in the scene
void Scene::moveObject(int object, char direction) {
    Transformation* transform = objects[object]->getTransformation();
    glm::vec3 currentPosition = transform->getPosition();  // Get the current position

    glm::vec3 movement(0.0f);
    float movementSpeed = 0.01f;

    switch (direction)
    {
        case 'u':
            movement = glm::vec3(0.0f, movementSpeed, 0.0f); // Move along Y-axis +
            break;

        case 'd':
            movement = glm::vec3(0.0f, -movementSpeed, 0.0f);// Move along Y-axis - 
            break;

        case 'l':
            movement = glm::vec3(-movementSpeed, 0.0f, 0.0f); // Move along X-axis + 
            break;

        case 'r':
			movement = glm::vec3(movementSpeed, 0.0f, 0.0f); // Move along X-axis - 
            break;

        case 'f':
            movement = glm::vec3(0.0f, 0.0f, movementSpeed); // Move along Z-axis +
			break;

        case 'b':
			movement = glm::vec3(0.0f, 0.0f, -movementSpeed); // Move along Z-axis -
			break;

        default:
            break;
    }

    // Move in global coordinatese
    transform->setPosition(currentPosition + movement);

}

void Scene::moveObject(int object, char direction, float amount) {
    Transformation* transform = objects[object]->getTransformation();
    glm::vec3 currentPosition = transform->getPosition();  // Get the current position

    glm::vec3 movement(0.0f);

    switch (direction)
    {
    case 'u':
        movement = glm::vec3(0.0f, amount, 0.0f); // Move along Y-axis +
        break;

    case 'd':
        movement = glm::vec3(0.0f, -amount, 0.0f);// Move along Y-axis - 
        break;

    case 'l':
        movement = glm::vec3(-amount, 0.0f, 0.0f); // Move along X-axis + 
        break;

    case 'r':
        movement = glm::vec3(amount, 0.0f, 0.0f); // Move along X-axis - 
        break;

    case 'f':
        movement = glm::vec3(0.0f, 0.0f, amount); // Move along Z-axis +
        break;

    case 'b':
        movement = glm::vec3(0.0f, 0.0f, -amount); // Move along Z-axis -
        break;

    default:
        break;
    }

    // Move in global coordinatese
    transform->setPosition(currentPosition + movement);
}

// Rotates the current object in the scene
void Scene::rotateObject(int currentObject, int direction) {
    Transformation* transform = objects[currentObject]->getTransformation();

    glm::mat4 rotationMatrix(1.0f);  // Identity matrix for rotation

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
	transform->setRotationMatrix(transform->getRotationMatrix() *  rotationMatrix);
}

// Scales the current object in the scene
void Scene::scaleObject(int currentObject, char direction) {
    Transformation* transform = objects[currentObject]->getTransformation();

	if (direction == 'u') { 
		transform->setScale(transform->getScale() + 0.01f); // Scale up
	}
	else if (direction == 'd') { 
        transform->setScale(transform->getScale() - 0.01f); // Scale down
    }
}

// Resets the rotation of the current object in the scene
void Scene::resetObjectRotation(int currentObject) {
    Transformation* transform = objects[currentObject]->getTransformation();
    transform->resetRotation();
}

// Resets the scale of the current object in the scene
void Scene::resetObjectScale(int currentObject) {
	Transformation* transform = objects[currentObject]->getTransformation();
	transform->resetScale();
}

// Returns the shaders used by the objects in the scene
vector<Shader*> Scene::getShaders() {
	vector<Shader*> shaders;

	for (int i = 0; i < objects.size(); i++) {
		shaders.push_back(objects[i]->getShader());
	}

	return shaders;
}