#include "Scene.h"

Scene::Scene() : currentObject(0), currentCamera(0) {
	addCamera();
}

Scene::~Scene() {
    for (auto* obj : objects) delete obj;
    for (auto* cam : cameras) delete cam;
    for (auto* light : lights) delete light;
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

DrawableObject* Scene::getObject(int index)
{
    return objects[index];
}

void Scene::addCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
{
	cameras.push_back(new Camera(position, up, yaw, pitch));
}

void Scene::addLight(std::shared_ptr<Model> model, LightType type)
{
    switch(type){
	    case LightType::POINT:
		    lights.push_back(new PointLight(model, 
                glm::vec3(0.0f),
			    glm::vec3(1.0f),
                1.0f));
		    break;
	    case LightType::DIRECTIONAL:
			lights.push_back(new DirectionalLight(glm::vec3(-1.0f, -1.0f, 0.0f),
		        glm::vec3(1.0f),
                1.0f));
		    break;
	    case LightType::SPOTLIGHT:
			lights.push_back(new SpotLight(model,
				cameras[currentCamera]->getPosition(),
				cameras[currentCamera]->getTarget(),
				glm::vec3(1.0f),
				1.0f,
                12.5f,
                17.5f));
		    break;
    }
}

void Scene::addLight(std::shared_ptr<Model> model, glm::vec3 position, glm::vec3 color, float intensity, LightType type)
{
    switch (type) {
    case LightType::POINT:
        lights.push_back(new PointLight(model,
            glm::vec3(0.0f),
            color,
            intensity));
        break;
    case LightType::DIRECTIONAL:
        lights.push_back(new DirectionalLight(glm::vec3(0.0f, -1.0f, 0.0f),
            color,
            intensity));
        break;
    case LightType::SPOTLIGHT:
        lights.push_back(new SpotLight(model,
            cameras[currentCamera]->getPosition(),
            cameras[currentCamera]->getTarget(),
            color,
            intensity,
            12.5f,
            17.5f));
        break;
    }
}

//Light* Scene::addLight(std::shared_ptr<Model> model, glm::vec3 position, glm::vec3 color, float intensity, LightType type, bool follow) {
//    Light* newLight = nullptr;
//    switch (type) {
//    case LightType::POINT:
//        newLight = new PointLight(model, position, color, intensity);
//        break;
//    case LightType::DIRECTIONAL:
//        newLight = new DirectionalLight(position, color, intensity);  // 'position' used as direction
//        break;
//    case LightType::SPOTLIGHT:
//        newLight = new SpotLight(model,
//            cameras[currentCamera]->getPosition(),
//            cameras[currentCamera]->getTarget(),
//            color,
//            intensity,
//            12.5f,
//            17.5f);
//        break;
//    }
//    if (newLight) {
//        lights.push_back(newLight);
//    }
//    return newLight;
//}

void Scene::setFollowingSpotLight(std::shared_ptr<Model> model)
{
    cameras[currentCamera]->setFollowingSpotlight(new SpotLight(
        model,
        cameras[currentCamera]->getPosition(),
        cameras[currentCamera]->getTarget(),
        glm::vec3(1.0f),
        1.0f,
        12.5f,
        17.5f));
    lights.push_back(cameras[currentCamera]->getFollowingSpotlight());
}

void Scene::setFollowingSpotLight(std::shared_ptr<Model> model, glm::vec3 color, float intensity)
{
    cameras[currentCamera]->setFollowingSpotlight(new SpotLight(
        model, 
		cameras[currentCamera]->getPosition(),
		cameras[currentCamera]->getTarget(),
		color,
		intensity,
		12.5f,
		17.5f));
	lights.push_back(cameras[currentCamera]->getFollowingSpotlight());
}

// Renders all objects in the scene
void Scene::render() {
    // Render all objects in the scene
    if (cameras[currentCamera]->isSetSkyBox()) {
        glDepthMask(GL_FALSE);  // Disable writing to the depth buffer
        glDisable(GL_DEPTH_TEST);  // Disable depth testing
		
        drawSkyBoxes(aspectRatio);
        
        glEnable(GL_DEPTH_TEST); 
        glDepthMask(GL_TRUE);   
    }
    
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
		if (camera->isSetSkyBox()) {
			camera->registerObserver((IObserver*)camera->getSkyBox()->getShader());
		}
    }

    for (Camera* camera : cameras) {
		camera->notifyObservers(aspectRatio, lights);
    }

}

void Scene::registerNewObserver(DrawableObject* newObject) {
	for (Camera* camera : cameras) {
		camera->registerObserver((IObserver*)newObject->getShader());
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

void Scene::setCurrentObject(int objectId)
{
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i]->getId() == objectId) {
			currentObject = i;
			break;
		}
	}
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
    if (object >= 0 && object < objects.size()) {
        switch (direction) {
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
	else printf("Object index out of bounds\n");
    
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

// Rotates object -> 1 = X, 2 = -X, 3 = Y, 4 = -Y, 5 = Z, 6 = -Z
void Scene::rotateObject(int object, int direction, float amount) {
    switch (direction) {
    case 1:
		objects[object]->rotateObject(glm::vec3(1.0f, 0.0f, 0.0f), amount); // Rotate +1 degree on X-axis
        break;

    case 2:
		objects[object]->rotateObject(glm::vec3(1.0f, 0.0f, 0.0f), -amount); // Rotate -1 degree on X-axis
        break;

    case 3:
		objects[object]->rotateObject(glm::vec3(0.0f, 1.0f, 0.0f), amount); // Rotate +1 degree on Y-axis
        break;

    case 4:
		objects[object]->rotateObject(glm::vec3(0.0f, 1.0f, 0.0f), -amount); // Rotate -1 degree on Y-axis
        break;

    case 5:
		objects[object]->rotateObject(glm::vec3(0.0f, 0.0f, 1.0f), amount); // Rotate +1 degree on Z-axis
        break;

    case 6:
		objects[object]->rotateObject(glm::vec3(0.0f, 0.0f, 1.0f), -amount); // Rotate -1 degree on Z-axis
        break;

    default:
        break;
    }
}

void Scene::setObjectRotation(int object, int direction, float amount) {
    switch (direction) {
    case 1:
        objects[object]->setRotation(glm::vec3(1.0f, 0.0f, 0.0f), amount); // Rotate +1 degree on X-axis
        break;

    case 2:
        objects[object]->setRotation(glm::vec3(1.0f, 0.0f, 0.0f), -amount); // Rotate -1 degree on X-axis
        break;

    case 3:
        objects[object]->setRotation(glm::vec3(0.0f, 1.0f, 0.0f), amount); // Rotate +1 degree on Y-axis
        break;

    case 4:
        objects[object]->setRotation(glm::vec3(0.0f, 1.0f, 0.0f), -amount); // Rotate -1 degree on Y-axis
        break;

    case 5:
        objects[object]->setRotation(glm::vec3(0.0f, 0.0f, 1.0f), amount); // Rotate +1 degree on Z-axis
        break;

    case 6:
        objects[object]->setRotation(glm::vec3(0.0f, 0.0f, 1.0f), -amount); // Rotate -1 degree on Z-axis
        break;

    default:
        break;
    }
}

// Scales the current object in the scene
void Scene::scaleObject(int object, char direction) {
    float scaleFactor = (direction == 'u') ? 1.01f : 0.99f;
    objects[object]->scaleObject(scaleFactor);
}

void Scene::setScaleObject(int object, float amount) {
    objects[object]->scaleObject(amount);
}

// Resets the rotation of the current object in the scene
void Scene::resetObjectRotation(int currentObject) {
    objects[currentObject]->setRotation(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);
    objects[currentObject]->setRotation(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
    objects[currentObject]->setRotation(glm::vec3(0.0f, 0.0f, 1.0f), 0.0f);   
}

// Resets the scale of the current object in the scene
void Scene::resetObjectScale(int currentObject) {
    objects[currentObject]->setScale(1.0f);
}

glm::vec3 Scene::getPositionLight(int light)
{
    return lights[light]->getPosition();
}

glm::vec3 Scene::getPositionObject(int object)
{
    return objects[object]->getPosition();
}

glm::mat4 Scene::getViewMatrix()
{
	return cameras[currentCamera]->getViewMatrix();
}

glm::mat4 Scene::getProjectionMatrix(float aspectRatio)
{
	return cameras[currentCamera]->getProjectionMatrix(aspectRatio);
}

LightType Scene::getLightType(int light)
{
	return lights[light]->getType();
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

void Scene::setSkyBox(DrawableObject* skybox)
{
	skyBoxSet = true;
	cameras[currentCamera]->setSkyBox(skybox);
    registerNewObserver(skybox);
}

void Scene::setSkyBox(DrawableObject* skybox, int camera)
{
	skyBoxSet = true;
	cameras[camera]->setSkyBox(skybox);
    registerNewObserver(skybox);
}

void Scene::toggleSkyBox()
{
	cameras[currentCamera]->toggleSkyBox();
	cout << "Skybox toggled for camera [" << currentCamera << "]: " << cameras[currentCamera]->isFollowingSkybox() << endl;
}

void Scene::toggleSkyBox(int camera)
{
	cameras[camera]->toggleSkyBox();
}

void Scene::setFollowingSkybox(bool follow)
{
    cameras[currentCamera]->setFollowingSkybox(follow);
}

void Scene::setFollowingSkybox(bool follow, int camera)
{
    cameras[camera]->setFollowingSkybox(follow);
}

void Scene::drawSkyBoxes(float aspectRatio)
{
    if (cameras[currentCamera]->isSetSkyBox()) {
        cameras[currentCamera]->drawSkyBox(aspectRatio);
    }
}

bool Scene::isFollowingSkybox()
{
	return cameras[currentCamera]->isFollowingSkybox();
}

bool Scene::isFollowingSkybox(int camera)
{
    return cameras[camera]->isFollowingSkybox();
}