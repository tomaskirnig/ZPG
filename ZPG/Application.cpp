#include "Application.h"

vector<string> vertexShaderSources = {
    "vertexShaderSource1", 
    "vertexShaderSource2", 
    "vertexShaderSource3", 
    "vertexShaderSource3" 
};
vector<string> fragmentShaderSources = {
    "fragmentShaderSource1", 
    "fragmentShaderSource2", 
    "fragmentShaderSource3", 
    "LambertFragmentShaderSource", 
    "PhongFragmentShaderSource",
    "PhongBlinnFragmentShaderSource"
};
int cntr = 0;

Application::Application() {
    if (!glfwInit()) {
        cerr << "Failed to initialize GLFW" << endl;
        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(1152, 648, "ZPG", NULL, NULL);
    if (!window) {
        glfwTerminate();
        cerr << "Failed to create GLFW window" << endl;
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cerr << "Failed to initialize GLEW" << endl;
        exit(EXIT_FAILURE);
    }

	// get version info
    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("Using GLEW %s\n", glewGetString(GLEW_VERSION));
    printf("Vendor %s\n", glGetString(GL_VENDOR));
    printf("Renderer %s\n", glGetString(GL_RENDERER));
    printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    printf("Using GLFW %i.%i.%i\n", major, minor, revision);

	// Callback functions
    glfwSetFramebufferSizeCallback(window, window_size_callback);
	glfwSetKeyCallback(window, key_callback);
    //glfwSetWindowFocusCallback(window, window_focus_callback);
	//glfwSetWindowIconifyCallback(window, window_iconify_callback);
	glfwSetCursorPosCallback(window, cursor_callback);
	//glfwSetMouseButtonCallback(window, button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetWindowUserPointer(window, this); // Creates pointer to app window

    // Settings of app window
    glfwGetFramebufferSize(window, &width, &height);
    aspectRatio = width / (float)height;
    glViewport(0, 0, width, height);	

    modelManager = ModelManager();

    addScene();
    scenes[0]->addCamera();

    currentScene = 0;
    
	lastX = width / 2;
	lastY = height / 2;
}

Application::~Application()
{
	for (Scene* scene : scenes) {
		delete scene;
	}
}

void Application::run() {
    float triangle[] = {
        -0.5f,  -0.3f, 0.0f,  0.5f,  0.5f, 0.0f,
        0.5f,  -0.3f, 0.0f,  0.0f,  0.0f, 1.0f,
        0.0f,  0.7f, 0.0f,  0.5f,  0.5f, 0.0f,
    };

    std::shared_ptr<Model> triangleModel = modelManager.getModel("triangle", triangle, sizeof(triangle));
	
	//vertexShaderSources[1], fragmentShaderSources[0]     // One color
	//vertexShaderSources[1], fragmentShaderSources[2]    // Normals as colors
	//vertexShaderSources[2], fragmentShaderSources[3-5]    // With lighting

    glEnable(GL_DEPTH_TEST);

	addScene();
    addScene();
    addScene();

    // Adding objects to first scene
    scenes[0]->addObject(new DrawableObject(triangleModel, vertexShaderSources[1], fragmentShaderSources[2]));

	// Add a forest to the second scene
	addForest(1, 50);

	addBalls(2);

    addBallsDiffShaders(3);
    
    registerAllObservers();

    // Hide and lock cursor in app window
	centerCursor();
	disableAndLockCursor();

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processInput();

        if (currentScene == 1) {
            scenes[currentScene]->rotateObject(0, 3);
            scenes[currentScene]->rotateObject(1, 3);
            scenes[currentScene]->rotateObject(2, 3);
			moveLightsRandom();
        }

		scenes[currentScene]->render();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

	enableCursor();
    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void Application::centerCursor() {
    // Calculate the center of the window
    double centerX = width / 2.0;
    double centerY = height / 2.0;

    // Set the cursor position to the center of the window
    glfwSetCursorPos(window, centerX, centerY);
}

void Application::disableAndLockCursor() {
    // Hide and grab the cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Application::enableCursor() {
    // Make the cursor visible and free it
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

float Application::getAspectRatio() {
	return aspectRatio;
}

// Change the current scene to the next one
void Application::currentScenePlus() {
    currentScene++;
    if (currentScene >= scenes.size()) {
        currentScene = 0; 
    }
    
	scenes[currentScene]->notifyCurrObservers(aspectRatio);

	cout << "Current scene: " << currentScene << endl;
}

// Change the current scene to the previous one
void Application::currentSceneMinus() {
    currentScene--;
    if (currentScene < 0) {
        currentScene = scenes.size() - 1;
    }

	scenes[currentScene]->notifyCurrObservers(aspectRatio);
    
	cout << "Current scene: " << currentScene << endl;
}

// Change the current object to the next one
void Application::currentObjectPlus() {
	scenes[currentScene]->currentObjectPlus();
}

void Application::currentObjectMinus() {
	scenes[currentScene]->currentObjectMinus();
}

void Application::currentCameraPlus() {
	scenes[currentScene]->currentCameraPlus(aspectRatio);
}

void Application::processInput() {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        scenes[currentScene]->moveCamera(scenes[currentScene]->getCurrCamera(), 'u', aspectRatio); // Move camera up
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        scenes[currentScene]->moveCamera(scenes[currentScene]->getCurrCamera(), 'd', aspectRatio); // Move camera down
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        scenes[currentScene]->moveCamera(scenes[currentScene]->getCurrCamera(), 'l', aspectRatio); // Move camera left
    }
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        scenes[currentScene]->moveCamera(scenes[currentScene]->getCurrCamera(), 'r', aspectRatio); // Move camera right
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        scenes[currentScene]->moveObject(scenes[currentScene]->getCurrObject(), 'u'); // Move object up
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        scenes[currentScene]->moveObject(scenes[currentScene]->getCurrObject(), 'd'); // Move object down
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        scenes[currentScene]->moveObject(scenes[currentScene]->getCurrObject(), 'l'); // Move object left
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        scenes[currentScene]->moveObject(scenes[currentScene]->getCurrObject(), 'r'); // Move object right
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        scenes[currentScene]->moveObject(scenes[currentScene]->getCurrObject(), 'b');  // Move object back
    }
    else if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        scenes[currentScene]->moveObject(scenes[currentScene]->getCurrObject(), 'f');  // Move object front
    }
	else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        scenes[currentScene]->rotateObject(scenes[currentScene]->getCurrObject(), 1);  // Rotate around x-axis
	}
	else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        scenes[currentScene]->rotateObject(scenes[currentScene]->getCurrObject(), 2);  // Rotate around x-axis
	}
	else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        scenes[currentScene]->rotateObject(scenes[currentScene]->getCurrObject(), 3);  // Rotate around y-axis
	}
	else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        scenes[currentScene]->rotateObject(scenes[currentScene]->getCurrObject(), 4);  // Rotate around y-axis
	}
	else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
        scenes[currentScene]->rotateObject(scenes[currentScene]->getCurrObject(), 5);  // Rotate around z-axis
	}
	else if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
        scenes[currentScene]->rotateObject(scenes[currentScene]->getCurrObject(), 6);  // Rotate around z-axis
	}
    else if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
        scenes[currentScene]->scaleObject(scenes[currentScene]->getCurrObject(), 'u'); // Scale up
    }
    else if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
        scenes[currentScene]->scaleObject(scenes[currentScene]->getCurrObject(), 'd');  // Scale down
    }
}

void Application::addScene() {
	scenes.push_back(new Scene());
}

void Application::registerAllObservers() {
    for (Scene* scene : scenes) {
        scene->registerAllObservers(aspectRatio);
    }
}

void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    //cout << "key_callback [" << key << "," << scancode << "," << action << "," << mods << "]" << endl;

    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));

    if (app == NULL) {
        cerr << "Key_callback -> app = NULL" << endl;
    }
    if (app) {
        if (action == GLFW_PRESS) {
            switch (key)
            {
                case GLFW_KEY_RIGHT:
					app->currentScenePlus();    // Change to the next scene
                    break;

                case GLFW_KEY_LEFT:
					app->currentSceneMinus();   // Change to the previous scene
                    break;
                
                case GLFW_KEY_UP:
					app->currentObjectPlus();   // Change to the next object
                    break;

                case GLFW_KEY_DOWN:
					app->currentObjectMinus();  // Change to the previous object
                    break;

				case GLFW_KEY_TAB:
					app->currentCameraPlus();   // Change to the next camera
					break;

				case GLFW_KEY_R:
					app->scenes[app->currentScene]->resetObjectRotation(app->scenes[app->currentScene]->getCurrObject()); // Reset object rotation
					break;

				case GLFW_KEY_T:
                    app->scenes[app->currentScene]->resetObjectScale(app->scenes[app->currentScene]->getCurrObject()); // Reset object scale

			    default:
				    break;
			}
        }
    }
}

// Other callback functions
void Application::error_callback(int error, const char* description) {
    fputs(description, stderr);
}

void Application::window_focus_callback(GLFWwindow* window, int focused) { cout << "window_focus_callback" << endl; }

void Application::window_iconify_callback(GLFWwindow* window, int iconified) { cout << "window_iconify_callback" << endl; }

void Application::window_size_callback(GLFWwindow* window, int width, int height) {
    //cout << "resize " << width << ", " << height << endl;
    glViewport(0, 0, width, height);
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window)); // Get pointer to the app window

	if (app == NULL) {
		cerr << "Window_size_callback -> app = NULL" << endl;
	}

	if (app) {
		// Resize the app window
		app->width = width;
		app->height = height;
		app->aspectRatio = width / (float)height;

        for (Scene* scene : app->scenes) {
            scene->setAspectRatio(app->aspectRatio);
        }

        // Notify camera of the aspect ratio change
        app->scenes[app->currentScene]->notifyCurrObservers(app->aspectRatio);
    }
}

void Application::cursor_callback(GLFWwindow* window, double x, double y) { 
    //cout << "cursor_callback" << "x: " << x << " y: " << y << endl;
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));

    if (app == NULL) {
        cerr << "Cursor_callback -> app = NULL" << endl;
    }
	if (app) {
		// Calculate the offset of the cursor position
        double xOffset = x - app->lastX;
        double yOffset =  app->lastY - y;

		// Update the last cursor position
        app->lastX = x;
        app->lastY = y;

		// Process the mouse movement
        app->scenes[app->currentScene]->mouseMovementCamera(app->scenes[app->currentScene]->getCurrCamera(), xOffset, yOffset, app->aspectRatio);
    }
}

void Application::button_callback(GLFWwindow* window, int button, int action, int mode) {
    if (action == GLFW_PRESS) {
		cout << "button_callback [" << button << "," << action << "," << mode << "]" << endl;
	}
}

void Application::scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));

    // Process the scroll input
    if (app != nullptr) {
        app->scenes[app->currentScene]->zoomCamera(app->scenes[app->currentScene]->getCurrCamera(), yOffset, app->aspectRatio);  // Pass the scroll input to your Application class
    }
}

// Functions for generating objects in scenes

void Application::addForest(int sceneIndex, int numTrees) {
    std::shared_ptr<Model> treeModel = modelManager.getModel("tree", tree, sizeof(tree));
	std::shared_ptr<Model> bushesModel = modelManager.getModel("bushes", bushes, sizeof(bushes));
	std::shared_ptr<Model> plainModel = modelManager.getModel("plain", plain, sizeof(plain));
	std::shared_ptr<Model> sphereModel = modelManager.getModel("sphere", sphere, sizeof(sphere));
    
    random_device rd;
    mt19937 gen(rd());  // Random number generator

    // Trees
    // Range for random positions
    uniform_real_distribution<> disX(-10.0, 10.0);  // X-axis range 
    uniform_real_distribution<> disZ(-20.0, 20.0);  // Z-axis range 

    // Random scaling
    uniform_real_distribution<> disScale(0.4, 1.0);

    // Random Y-axis rotation (0 to 360 degrees in radians)
    uniform_real_distribution<> disRotationY(0.0, 360.0);

    std::vector<glm::mat4> treeTransformations;

    // Place Trees
    for (int i = 0; i < numTrees; ++i) {
        // Generate random x, z positions 
        float randomX = disX(gen);
        float randomZ = disZ(gen);
        float randomRotationY = disRotationY(gen);  // Random rotation in radians

        DrawableObject* treeObject = new DrawableObject(treeModel, vertexShaderSources[2], fragmentShaderSources[3]);

        // Set the transformation matrix (position and scale) 
        treeObject->setPosition(glm::vec3(randomX, -0.5, randomZ));  // Place at random x, y, z 
        treeObject->setScale(disScale(gen));  // Apply random scaling
        treeObject->setRotation(glm::vec3(0.0f, 1.0f, 0.0f), randomRotationY);  // Rotate around Y-axis

        // Add the tree to the specified scene
        scenes[sceneIndex]->addObject(treeObject);
    }

    // Bushes
    for (int i = 0; i < numTrees; ++i) {
        float randomX = disX(gen);
        float randomZ = disZ(gen);
        float randomRotationY = disRotationY(gen);  // Random rotation in radians

        DrawableObject* bushObject = new DrawableObject(bushesModel, vertexShaderSources[2], fragmentShaderSources[3]);

        // Set the transformation matrix (position and scale) 
        bushObject->setPosition(glm::vec3(randomX, -0.5, randomZ));  // Place at random x, y, z 
        bushObject->setScale(disScale(gen));  // Apply random scaling
        bushObject->setRotation(glm::vec3(0.0f, 1.0f, 0.0f), randomRotationY);  // Rotate around Y-axis

        // Add the tree to the specified scene
        scenes[sceneIndex]->addObject(bushObject);
    }

    DrawableObject* plainObj = new DrawableObject(plainModel, vertexShaderSources[2], fragmentShaderSources[5]);
    plainObj->setPosition(glm::vec3(0.0, -0.5, 0.0));
    plainObj->setScale(20.0);

    scenes[sceneIndex]->addObject(plainObj);

    for (int i = 0; i < 4; i++) {
        float randomX = disX(gen);
        float randomZ = disZ(gen);
        scenes[sceneIndex]->addLight(sphereModel, glm::vec3(randomX, 0.0, randomZ), glm::vec3(1.0f, 0.5f, 0.1f), 1.0f);
    }
}

//void Application::addForest(int sceneIndex, int numTrees) {
//    std::shared_ptr<Model> treeModel = modelManager.getModel("tree", tree, sizeof(tree));
//    std::shared_ptr<Model> bushModel = modelManager.getModel("bushes", bushes, sizeof(bushes));
//	std::shared_ptr<Model> plainModel = modelManager.getModel("plain", plain, sizeof(plain));
//	std::shared_ptr<Model> sphereModel = modelManager.getModel("sphere", sphere, sizeof(sphere));
//
//    std::random_device rd;
//    std::mt19937 gen(rd());
//    std::uniform_real_distribution<> disX(-10.0, 10.0);
//    std::uniform_real_distribution<> disZ(-20.0, 20.0);
//    std::uniform_real_distribution<> disScale(0.4, 1.0);
//    std::uniform_real_distribution<> disRotationY(0.0, 360.0);
//
//    // Collect transformations
//    std::vector<glm::mat4> treeTransforms, bushTransforms;
//
//    for (int i = 0; i < numTrees; ++i) {
//        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(disX(gen), -0.5f, disZ(gen))) *
//            glm::rotate(glm::mat4(1.0f), glm::radians<float>(disRotationY(gen)), glm::vec3(0.0f, 1.0f, 0.0f)) *
//            glm::scale(glm::mat4(1.0f), glm::vec3(disScale(gen)));
//        treeTransforms.push_back(transform);
//    }
//
//    for (int i = 0; i < numTrees; ++i) {
//        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(disX(gen), -0.5f, disZ(gen))) *
//            glm::rotate(glm::mat4(1.0f), glm::radians<float>(disRotationY(gen)), glm::vec3(0.0f, 1.0f, 0.0f)) *
//            glm::scale(glm::mat4(1.0f), glm::vec3(disScale(gen)));
//        bushTransforms.push_back(transform);
//    }
//
//    // Add trees with instanced rendering
//    DrawableObject* treeObject = new DrawableObject(treeModel, vertexShaderSources[3], fragmentShaderSources[4]);
//    treeObject->setupInstancedRendering(treeTransforms);
//    scenes[sceneIndex]->addObject(treeObject);
//
//    // Add bushes with instanced rendering
//    DrawableObject* bushObject = new DrawableObject(bushModel, vertexShaderSources[3], fragmentShaderSources[4]);
//    bushObject->setupInstancedRendering(bushTransforms);
//    scenes[sceneIndex]->addObject(bushObject);
//
//    for (int i = 0; i < 4; i++) {
//        float randomX = disX(gen);
//        float randomZ = disZ(gen);
//        glm::vec3 position(randomX, 0.0f, randomZ);
//        glm::vec3 color((i % 2) * 1.0f, (i % 3) * 1.0f, (i / 3) * 1.0f);
//        scenes[sceneIndex]->addLight(sphereModel, position, color, 1.8f);
//    }
//}


void Application::addBalls(int sceneIndex) {
	std::shared_ptr<Model> sphereModel = modelManager.getModel("sphere", sphere, sizeof(sphere));
	scenes[sceneIndex]->addLight(sphereModel);

    int numOfObjectInScene = scenes[sceneIndex]->objectsCount();

	scenes[sceneIndex]->moveLight(0, 'd', 2.0f);
    for (int i = 0; i < 4; i++) {
        scenes[sceneIndex]->addObject(new DrawableObject(sphereModel, vertexShaderSources[2], fragmentShaderSources[4]));
    }
    
    scenes[sceneIndex]->moveObject(numOfObjectInScene++, 'u', 2.0);
    scenes[sceneIndex]->moveObject(numOfObjectInScene++, 'd', 2.0);
    scenes[sceneIndex]->moveObject(numOfObjectInScene++, 'l', 2.0);
    scenes[sceneIndex]->moveObject(numOfObjectInScene, 'r', 2.0);
}

void Application::addMonkeys(int sceneIndex) {
	std::shared_ptr<Model> suziFlatModel = modelManager.getModel("suziFlat", suziFlat, sizeof(suziFlat));
	std::shared_ptr<Model> sphereModel = modelManager.getModel("sphere", sphere, sizeof(sphere));
    
    scenes[sceneIndex]->addLight(sphereModel);

	int numOfObjectInScene = scenes[sceneIndex]->objectsCount();
	for (int i = 0; i < 4; i++) {
		scenes[sceneIndex]->addObject(new DrawableObject(suziFlatModel, vertexShaderSources[2], fragmentShaderSources[i]));
	}

	scenes[sceneIndex]->moveObject(numOfObjectInScene++, 'u', 2.0);
	scenes[sceneIndex]->moveObject(numOfObjectInScene++, 'd', 2.0);
	scenes[sceneIndex]->moveObject(numOfObjectInScene++, 'l', 2.0);
	scenes[sceneIndex]->moveObject(numOfObjectInScene, 'r', 2.0);
}

void Application::addBallsDiffShaders(int sceneIndex) {
	std::shared_ptr<Model> sphereModel = modelManager.getModel("sphere", sphere, sizeof(sphere));
    scenes[sceneIndex]->addLight(sphereModel, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.5f, 0.0f), 1.0f);

    int numOfObjectInScene = scenes[sceneIndex]->objectsCount();
    scenes[sceneIndex]->addObject(new DrawableObject(sphereModel, vertexShaderSources[1], fragmentShaderSources[2]));

    for (int i = 3; i < 6; i++) {
        scenes[sceneIndex]->addObject(new DrawableObject(sphereModel, vertexShaderSources[2], fragmentShaderSources[i], 130.0f));
    }

    scenes[sceneIndex]->moveObject(numOfObjectInScene++, 'u', 2.0);
    scenes[sceneIndex]->moveObject(numOfObjectInScene++, 'd', 2.0);
    scenes[sceneIndex]->moveObject(numOfObjectInScene++, 'l', 2.0);
    scenes[sceneIndex]->moveObject(numOfObjectInScene, 'r', 2.0);
}

void Application::moveLightsRandom() {
    static std::vector<char> directions = { 'u', 'd', 'l', 'r', 'f', 'b' };
    static std::mt19937 gen(std::random_device{}());
    static std::vector<int> randomDirections(scenes[currentScene]->getNumOfLights()); // Direction for each light
    static std::uniform_int_distribution<> dis(0, 5);  // Random direction generator

    if (cntr == 0) {
        // Assign a new random direction for each light when counter is 0
        for (int i = 0; i < randomDirections.size(); ++i) {
            randomDirections[i] = dis(gen);
        }
        cntr++;
    }
    else {
        // Move each light independently within bounds
        for (int i = 0; i < scenes[currentScene]->getNumOfLights(); i++) {
            glm::vec3 currentPosition = scenes[currentScene]->getPositionLight(i);
            char direction = directions[randomDirections[i]];

            // Check bounds for the selected direction
            switch (direction) {
            case 'u':  // Move up (Y)
                if (currentPosition.y < 0.8f) {
                    scenes[currentScene]->moveLight(i, 'u', 0.01f);
                }
                break;
            case 'd':  // Move down (Y)
                if (currentPosition.y > -0.4f) {
                    scenes[currentScene]->moveLight(i, 'd', 0.01f);
                }
                break;
            case 'l':  // Move left (X)
                if (currentPosition.x > -10.0f) {
                    scenes[currentScene]->moveLight(i, 'l', 0.01f);
                }
                break;
            case 'r':  // Move right (X)
                if (currentPosition.x < 10.0f) {
                    scenes[currentScene]->moveLight(i, 'r', 0.01f);
                }
                break;
            case 'f':  // Move forward (Z)
                if (currentPosition.z < 20.0f) {
                    scenes[currentScene]->moveLight(i, 'f', 0.01f);
                }
                break;
            case 'b':  // Move backward (Z)
                if (currentPosition.z > -20.0f) {
                    scenes[currentScene]->moveLight(i, 'b', 0.01f);
                }
                break;
            }
        }

        // Increment and reset counter after 100 moves
        cntr++;
        if (cntr == 100) {
            cntr = 0;
        }
		scenes[currentScene]->notifyCurrObservers(aspectRatio);
    }
}



