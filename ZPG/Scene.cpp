#include "Scene.h"

Scene::Scene() : currentObject(0), currentCamera(0) {
	addCamera();
	addLight(glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
}

Scene::~Scene()
{
	for (DrawableObject* object : objects) {
		delete object;
	}
	objects.clear();
}

// Adds a new object to the scene
void Scene::addObject(DrawableObject* object) {
    objects.push_back(object);
}

// Deletes an object from the scene
void Scene::deleteObject(DrawableObject* delObject) {
    for (auto obj = objects.begin(); obj != objects.end(); obj++) {
        if (*obj == delObject) {
            objects.erase(obj);
            break;
        }
    }
}

void Scene::addCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
{
	cameras.push_back(new Camera(position, up, yaw, pitch));
}


void Scene::addLight(glm::vec3 position, glm::vec3 color)
{
	lights.push_back(new Light());
	lights[lights.size() - 1]->setPosition(position);
	lights[lights.size() - 1]->setColor(color);
}

void Scene::addLight(glm::vec3 position, glm::vec3 color, float intensity)
{
    lights.push_back(new Light());
    lights[lights.size() - 1]->setPosition(position);
    lights[lights.size() - 1]->setColor(color);
	lights[lights.size() - 1]->setIntensity(intensity);
}

// Renders all objects in the scene
void Scene::render() {
    // Apply camera's view and projection matrices
    //cameras[currentCamera].notifyObservers(aspectRatio, lights);

	// Render all objects in the scene
    for (DrawableObject* object : objects) {
        object->draw();
    }
    for (Light* light : lights) {
		light->draw();
    }
}

void Scene::registerAllObservers(float aspectRatio)
{
    for (Camera* camera : cameras) {
        for (DrawableObject* object : objects) {
            camera->registerObserver((IObserver*)object->getShader());
        }
        for (Light* light : lights) {
			camera->registerObserver((IObserver*)light->getShader());
        }
    }

    for (Camera* camera : cameras) {
		camera->notifyObservers(aspectRatio, lights);
    }
}

void Scene::notifyCurrObservers(float aspectRatio)
{
	cameras[currentCamera]->notifyObservers(aspectRatio, lights);
}

// Returns the number of objects in the scene
int Scene::objectsCount() {
	return objects.size();
}

int Scene::getCurrCamera()
{
    return currentCamera;
}

int Scene::getCurrObject()
{
    return currentObject;
}

int Scene::getNumOfLights()
{
	return (int)lights.size();
}

void Scene::setAspectRatio(float aspectRatio)
{
    this->aspectRatio = aspectRatio;
}

void Scene::currentObjectPlus()
{
	currentObject++;
	if (currentObject >= objects.size()) {
		currentObject = 0;
	}
    cout << "Current object: " << currentObject << endl;
}

void Scene::currentObjectMinus()
{
	currentObject--;
	if (currentObject < 0) {
		currentObject = objects.size() - 1;
	}
    cout << "Current object: " << currentObject << endl;
}

void Scene::currentCameraPlus(float aspectRatio)
{
    currentCamera++;
    if (currentCamera >= cameras.size()) {
        currentCamera = 0;
    }
    cout << "Current camera: " << currentCamera << endl;

	notifyCurrObservers(aspectRatio);
}

// Moves the current object in the scene
void Scene::moveObject(int object, char direction) {
    float movementSpeed = 0.01f;

    switch (direction)
    {
    case 'u':
        objects[object]->moveObject(glm::vec3(0.0f, movementSpeed, 0.0f)); // Move + Y-axis
        break;

    case 'd':
        objects[object]->moveObject(glm::vec3(0.0f, -movementSpeed, 0.0f)); // Move - Y-axis
        break;

    case 'l':
        objects[object]->moveObject(glm::vec3(-movementSpeed, 0.0f, 0.0f)); // Move + X-axis
        break;

    case 'r':
        objects[object]->moveObject(glm::vec3(movementSpeed, 0.0f, 0.0f)); // Move - X-axis
        break;

    case 'f':
        objects[object]->moveObject(glm::vec3(0.0f, 0.0f, movementSpeed)); // Move + Z-axis
        break;

    case 'b':
        objects[object]->moveObject(glm::vec3(0.0f, 0.0f, -movementSpeed)); // Move - Z-axis
        break;
    default:
        break;
    }
}

void Scene::moveObject(int object, char direction, float amount) {
    switch (direction)
    {
    case 'u':
		objects[object]->moveObject(glm::vec3(0.0f, amount, 0.0f)); // Move + Y-axis
        break;

    case 'd':
		objects[object]->moveObject(glm::vec3(0.0f, -amount, 0.0f)); // Move - Y-axis
        break;

    case 'l':
		objects[object]->moveObject(glm::vec3(-amount, 0.0f, 0.0f)); // Move + X-axis
        break;

    case 'r':
		objects[object]->moveObject(glm::vec3(amount, 0.0f, 0.0f)); // Move - X-axis
        break;

    case 'f':
		objects[object]->moveObject(glm::vec3(0.0f, 0.0f, amount)); // Move + Z-axis
        break;

    case 'b':
		objects[object]->moveObject(glm::vec3(0.0f, 0.0f, -amount)); // Move - Z-axis
        break;

    default:
        break;
    }
}

void Scene::moveLight(int light, char direction) {
    float movementSpeed = 0.01f;

    switch (direction)
    {
    case 'u':
        lights[light]->moveObject(glm::vec3(0.0f, movementSpeed, 0.0f)); // Move + Y-axis
        break;

    case 'd':
        lights[light]->moveObject(glm::vec3(0.0f, -movementSpeed, 0.0f)); // Move - Y-axis
        break;

    case 'l':
        lights[light]->moveObject(glm::vec3(-movementSpeed, 0.0f, 0.0f)); // Move + X-axis
        break;

    case 'r':
        lights[light]->moveObject(glm::vec3(movementSpeed, 0.0f, 0.0f)); // Move - X-axis
        break;

    case 'f':
        lights[light]->moveObject(glm::vec3(0.0f, 0.0f, movementSpeed)); // Move + Z-axis
        break;

    case 'b':
        lights[light]->moveObject(glm::vec3(0.0f, 0.0f, -movementSpeed)); // Move - Z-axis
        break;

    default:
        break;
    }
}

void Scene::moveLight(int light, char direction, float amount) {
    switch (direction)
    {
    case 'u':
        lights[light]->moveObject(glm::vec3(0.0f, amount, 0.0f)); // Move + Y-axis
        break;

    case 'd':
        lights[light]->moveObject(glm::vec3(0.0f, -amount, 0.0f)); // Move - Y-axis
        break;

    case 'l':
        lights[light]->moveObject(glm::vec3(-amount, 0.0f, 0.0f)); // Move + X-axis
        break;

    case 'r':
        lights[light]->moveObject(glm::vec3(amount, 0.0f, 0.0f)); // Move - X-axis
        break;

    case 'f':
        lights[light]->moveObject(glm::vec3(0.0f, 0.0f, amount)); // Move + Z-axis
        break;

    case 'b':
        lights[light]->moveObject(glm::vec3(0.0f, 0.0f, -amount)); // Move - Z-axis
        break;

    default:
        break;
    }
}

// Rotates the current object in the scene
void Scene::rotateObject(int object, int direction) {
    switch (direction) {
    case 1:
		objects[object]->rotateObject(glm::vec3(1.0f, 0.0f, 0.0f), 1.0f); // Rotate +1 degree on X-axis
        break;

    case 2:
		objects[object]->rotateObject(glm::vec3(1.0f, 0.0f, 0.0f), -1.0f); // Rotate -1 degree on X-axis
        break;

    case 3:
		objects[object]->rotateObject(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f); // Rotate +1 degree on Y-axis
        break;

    case 4:
		objects[object]->rotateObject(glm::vec3(0.0f, 1.0f, 0.0f), -1.0f); // Rotate -1 degree on Y-axis
        break;

    case 5:
		objects[object]->rotateObject(glm::vec3(0.0f, 0.0f, 1.0f), 1.0f); // Rotate +1 degree on Z-axis
        break;

    case 6:
		objects[object]->rotateObject(glm::vec3(0.0f, 0.0f, 1.0f), -1.0f); // Rotate -1 degree on Z-axis
        break;

    default:
        break;
    }
}

// Scales the current object in the scene
void Scene::scaleObject(int object, char direction) {
	if (direction == 'u') { 
        objects[object]->scaleObject(0.01f);
	}
	else if (direction == 'd') { 
		objects[object]->scaleObject(-0.01f);
    }
}

// Resets the rotation of the current object in the scene
void Scene::resetObjectRotation(int currentObject) {
    objects[currentObject]->setScale(1.0f);
}

// Resets the scale of the current object in the scene
void Scene::resetObjectScale(int currentObject) {
    objects[currentObject]->setRotation(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);
    objects[currentObject]->setRotation(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
    objects[currentObject]->setRotation(glm::vec3(0.0f, 0.0f, 1.0f), 0.0f);
}

glm::vec3 Scene::getPositionLight(int light)
{
    return lights[light]->getPosition();
}

glm::vec3 Scene::getPositionObject(int object)
{
    return objects[object]->getPosition();
}

void Scene::moveCamera(int camera, char direction, float aspectRatio)
{
	cameras[camera]->processKeyboardMovement(direction, aspectRatio, lights);
}

void Scene::mouseMovementCamera(int camera, float xOffset, float yOffset, float aspectRatio)
{
	cameras[camera]->processMouseMovement(xOffset, yOffset, aspectRatio, lights);
}

void Scene::zoomCamera(int camera, double yOffset, float aspectRatio)
{
	cameras[camera]->processMouseScroll(yOffset, aspectRatio, lights);
}

// Returns the shaders used by the objects in the scene
vector<Shader*> Scene::getShaders() {
	vector<Shader*> shaders;

	for (int i = 0; i < objectsCount(); i++) {
		shaders.push_back(objects[i]->getShader());
	}

	return shaders;
}