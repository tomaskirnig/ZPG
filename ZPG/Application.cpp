#include "Application.h"

vector<string> vertexShaderSources = {
    "vertexShaderSource1", 
    "vertexShaderSource2", 
    "vertexShaderSource3", 
    "vertexShaderSource4",
    "vertexShaderCubemap"
};
vector<string> fragmentShaderSources = {
    "fragmentShaderSource1", 
    "fragmentShaderSource2", 
    "fragmentShaderSource3", 
    "LambertFragmentShaderSource", 
    "PhongFragmentShaderSource",
    "PhongBlinnFragmentShaderSource",
    "CubemapFragmentShaderSource"
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
	textureManager = TextureManager();

	addScene(); // Triangle
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

    std::shared_ptr<Model> triangleModel = modelManager.getModel("triangle", triangle, sizeof(triangle), 1);
	
	//vertexShaderSources[1], fragmentShaderSources[0]     // One color
	//vertexShaderSources[1], fragmentShaderSources[2]    // Normals as colors
	//vertexShaderSources[2], fragmentShaderSources[3-5]    // With lighting
	//vertexShaderSources[3], fragmentShaderSources[6]    // Skybox

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    /*float* zBuffer = new float[width * height];
    glReadPixels(0, 0, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, zBuffer);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = y * width + x;
            std::cout << zBuffer[index] << " ,";
        }
		std::cout << std::endl;
    }
    delete[] zBuffer;*/
    
	addScene(); // Forest
	addScene(); // Balls
	addScene(); // Balls with different shaders
	addScene(); // Textures

    // Adding objects to first scene
    scenes[0]->addObject(new DrawableObject(triangleModel, vertexShaderSources[1], fragmentShaderSources[2], true));

	// Add a forest to the second scene
	addForest(1, 50);

	addBalls(2);

    addBallsDiffShaders(3);

	addTextures(4);
    
    registerAllObservers();

	window_size_callback(window, width, height);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        

        if (currentScene == 1) {
            scenes[currentScene]->rotateObject(0, 3);
            scenes[currentScene]->rotateObject(1, 3);
            scenes[currentScene]->rotateObject(2, 3);
			moveLightsRandom();
        }        

		scenes[currentScene]->notifyCurrObservers(aspectRatio);

		scenes[currentScene]->render();

        processInput();

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
	cout << "Current scene: " << currentScene << endl;
}

// Change the current scene to the previous one
void Application::currentSceneMinus() {
    currentScene--;
    if (currentScene < 0) {
        currentScene = scenes.size() - 1;
    }
	cout << "Current scene: " << currentScene << endl;
}

// Change the current object to the next one
void Application::currentObjectPlus() {
	scenes[currentScene]->currentObjectPlus();
}

void Application::currentObjectMinus() {
	scenes[currentScene]->currentObjectMinus();
}

void Application::setCurrentObject(int object)
{
	scenes[currentScene]->setCurrentObject(object);
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
    if (glfwGetKey(window, 340) == GLFW_PRESS) {
        // Hide and lock cursor in app window
        //centerCursor();
        disableAndLockCursor();
		looking = true;
    }
    else {
		enableCursor();
		looking = false;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1)) {
		double x, y;
		glfwGetCursorPos(window, &x, &y);
        handleClick(x, y);
    }
}

void Application::handleClick(double x, double y) {
    GLbyte color[4];
    GLfloat depth;
    GLuint id;

    int newy = height - y;

    glReadPixels(x, newy, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
    glReadPixels(x, newy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    glReadPixels(x, newy, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &id);

    printf("Clicked on pixel %d, %d, color %02hhx%02hhx%02hhx%02hhx, depth %.2f, stencil index % u\n", x, y, color[0], color[1], color[2], color[3], depth, id);
    
    lastClickX = x;
    lastClickY = newy;
    lastClickDepth = depth;
	
    selectObject(id);
}

void Application::selectObject(GLuint id) {
	if (id == 0) {
		cout << "No object selected" << endl;
	}
	else {
		cout << "Object with index " << id << " selected" << endl;
		scenes[currentScene]->setCurrentObject(id);
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
                    break;

                case GLFW_KEY_LEFT_ALT:
					app->scenes[app->currentScene]->toggleSkyBox(); 
					break;
                case GLFW_KEY_N:
					app->createSphereAtClick();
					break;
				case GLFW_KEY_DELETE:
					app->removeSelectedObject();
					break;

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
    }
}

void Application::cursor_callback(GLFWwindow* window, double x, double y) { 
    //cout << "cursor_callback" << "x: " << x << " y: " << y << endl;
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));

    if (app->looking) {
        if (app == NULL) {
            cerr << "Cursor_callback -> app = NULL" << endl;
        }
        if (app) {
            // Calculate the offset of the cursor position
            double xOffset = x - app->lastX;
            double yOffset = app->lastY - y;

            // Update the last cursor position
            app->lastX = x;
            app->lastY = y;

            // Process the mouse movement
            app->scenes[app->currentScene]->mouseMovementCamera(app->scenes[app->currentScene]->getCurrCamera(), xOffset, yOffset, app->aspectRatio);
        }
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
    std::shared_ptr<Model> treeModel = modelManager.getModel("tree", tree, sizeof(tree), 1);
	std::shared_ptr<Model> bushesModel = modelManager.getModel("bushes", bushes, sizeof(bushes), 1);
	std::shared_ptr<Model> sphereModel = modelManager.getModel("sphere", sphere, sizeof(sphere), 1);
    std::shared_ptr<Model> plainTextureModel = modelManager.getModel("plain_texture", plain_texture, sizeof(plain_texture), 2);
    std::shared_ptr<Model> skycubeModel = modelManager.getModel("skycube", skycube, sizeof(skycube), 0);
	std::shared_ptr<Model> houseModel = modelManager.getModel("building", building, sizeof(building), 2);
	std::shared_ptr<Model> skydomeModel = modelManager.getModel("skydome", skydome, sizeof(skydome), 2);

	std::shared_ptr<Model> zombieModel = modelManager.getModel("zombie.obj");
	std::shared_ptr<Model> toiletModel = modelManager.getModel("toilet.obj");
    std::shared_ptr<Model> loginModel = modelManager.getModel("login.obj");

    std::shared_ptr<Texture> grassTexture = textureManager.getTexture("grass.png");
    std::shared_ptr<Texture> skyTexture = textureManager.getCubeMap();
	std::shared_ptr<Texture> houseTexture = textureManager.getTexture("house.png");
	std::shared_ptr<Texture> zombieTexture = textureManager.getTexture("zombie.png");
	std::shared_ptr<Texture> toiletTexture = textureManager.getTexture("toilet.png");
	std::shared_ptr<Texture> skydomeTexture = textureManager.getTexture("skydome.png");

    Material* skyDomeMaterial = new Material(glm::vec3(0.1f), glm::vec3(0.1f), glm::vec3(0.1f), 1.0f, skydomeTexture);
    Material* houseMaterial = new Material(glm::vec3(0.1f), glm::vec3(0.1f), glm::vec3(0.1f), 1.0f, houseTexture);
    Material* zombieMaterial = new Material(glm::vec3(0.1f), glm::vec3(0.1f), glm::vec3(0.1f), 1.0f, zombieTexture);
    Material* toiletMaterial = new Material(glm::vec3(0.1f), glm::vec3(0.1f), glm::vec3(0.1f), 1.0f, toiletTexture);
    Material* skycubeMaterial = new Material(glm::vec3(0.1f), glm::vec3(0.1f), glm::vec3(0.1f), 1.0f, skyTexture);
	
    scenes[sceneIndex]->setSkyBox(new DrawableObject(skycubeModel, vertexShaderSources[4], fragmentShaderSources[6], false));

    random_device rd;
    mt19937 gen(rd());  // Random number generator

    // Trees
    // Range for random positions
    uniform_real_distribution<> disX(-10.0, 0.0);  // X-axis range 
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

        DrawableObject* treeObject = new DrawableObject(treeModel, vertexShaderSources[2], fragmentShaderSources[5], false);

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

        DrawableObject* bushObject = new DrawableObject(bushesModel, vertexShaderSources[2], fragmentShaderSources[5], false);

        // Set the transformation matrix (position and scale) 
        bushObject->setPosition(glm::vec3(randomX, -0.5, randomZ));  // Place at random x, y, z 
        bushObject->setScale(disScale(gen));  // Apply random scaling
        bushObject->setRotation(glm::vec3(0.0f, 1.0f, 0.0f), randomRotationY);  // Rotate around Y-axis

        // Add the tree to the specified scene
        scenes[sceneIndex]->addObject(bushObject);
    }
    Material* material = new Material(glm::vec3(0.1f), glm::vec3(0.1f), glm::vec3(0.1f), 1.0f, grassTexture);
    DrawableObject* plainObj = new DrawableObject(plainTextureModel, vertexShaderSources[3], fragmentShaderSources[3], material, true);

    plainObj->setPosition(glm::vec3(0.0, -0.5, 0.0));
    plainObj->setScale(20.0);

    scenes[sceneIndex]->addObject(plainObj);

    for (int i = 0; i < 3; i++) {
        float randomX = disX(gen);
        float randomZ = disZ(gen);
        scenes[sceneIndex]->addLight(sphereModel, glm::vec3(randomX, 0.0, randomZ), glm::vec3(1.0f, 0.5f, 0.1f), 1.0f, LightType::POINT);
    }
    scenes[sceneIndex]->setFollowingSpotLight(sphereModel);
    //scenes[sceneIndex]->addLight(nullptr, LightType::DIRECTIONAL); 

    scenes[sceneIndex]->addObject(new DrawableObject(loginModel, vertexShaderSources[3], fragmentShaderSources[3], material, true));
    scenes[sceneIndex]->moveObject(scenes[sceneIndex]->objectsCount() - 1, 'u', 1.0f);

	scenes[sceneIndex]->addObject(new DrawableObject(houseModel, vertexShaderSources[3], fragmentShaderSources[3], houseMaterial, true));
    scenes[sceneIndex]->moveObject(scenes[sceneIndex]->objectsCount() - 1, 'r', 15.0f);
    scenes[sceneIndex]->moveObject(scenes[sceneIndex]->objectsCount() - 1, 'd', 0.5f);

	scenes[sceneIndex]->addObject(new DrawableObject(zombieModel, vertexShaderSources[3], fragmentShaderSources[3], zombieMaterial, true));
    scenes[sceneIndex]->moveObject(scenes[sceneIndex]->objectsCount() - 1, 'r', 15.0f);
    scenes[sceneIndex]->moveObject(scenes[sceneIndex]->objectsCount() - 1, 'f', 15.0f);
    scenes[sceneIndex]->moveObject(scenes[sceneIndex]->objectsCount() - 1, 'd', 0.5f);
	scenes[sceneIndex]->setObjectRotation(scenes[sceneIndex]->objectsCount() - 1, 4, 90.0f);


	scenes[sceneIndex]->addObject(new DrawableObject(toiletModel, vertexShaderSources[3], fragmentShaderSources[3], toiletMaterial, true));
    scenes[sceneIndex]->moveObject(scenes[sceneIndex]->objectsCount() - 1, 'r', 7.0f);
    scenes[sceneIndex]->moveObject(scenes[sceneIndex]->objectsCount() - 1, 'f', 2.0f);
    scenes[sceneIndex]->moveObject(scenes[sceneIndex]->objectsCount() - 1, 'd', 0.5f);
    scenes[sceneIndex]->setScaleObject(scenes[sceneIndex]->objectsCount() - 1, 0.2f);

	/*scenes[sceneIndex]->addObject(new DrawableObject(skydomeModel, vertexShaderSources[3], fragmentShaderSources[3], skyDomeMaterial));
	scenes[sceneIndex]->moveObject(scenes[sceneIndex]->objectsCount() - 1, 'd', 5.0f);
    scenes[sceneIndex]->setScaleObject(scenes[sceneIndex]->objectsCount() - 1, 10.0f);*/

    scenes[sceneIndex]->setSkyBox(new DrawableObject(skycubeModel, vertexShaderSources[4], fragmentShaderSources[6], skycubeMaterial));
}

void Application::addBalls(int sceneIndex) {
	std::shared_ptr<Model> sphereModel = modelManager.getModel("sphere", sphere, sizeof(sphere), 1);
	scenes[sceneIndex]->addLight(sphereModel, LightType::POINT);

    int numOfObjectInScene = scenes[sceneIndex]->objectsCount();

    for (int i = 0; i < 4; i++) {
        scenes[sceneIndex]->addObject(new DrawableObject(sphereModel, vertexShaderSources[2], fragmentShaderSources[5], false));
    }
    
    scenes[sceneIndex]->moveObject(numOfObjectInScene++, 'u', 2.0);
    scenes[sceneIndex]->moveObject(numOfObjectInScene++, 'd', 2.0);
    scenes[sceneIndex]->moveObject(numOfObjectInScene++, 'l', 2.0);
    scenes[sceneIndex]->moveObject(numOfObjectInScene, 'r', 2.0);
}

void Application::addTextures(int sceneIndex)
{
	std::shared_ptr<Model> plainTextureModel = modelManager.getModel("plain_texture", plain_texture, sizeof(plain_texture), 2);
    std::shared_ptr<Model> skyCubeModel = modelManager.getModel("skycube", skycube, sizeof(skycube), 0);
    std::shared_ptr<Model> skydomeModel = modelManager.getModel("skydome", skydome, sizeof(skydome), 2);

	std::shared_ptr<Model> zombie = modelManager.getModel("zombie.obj");
    std::shared_ptr<Model> toilet = modelManager.getModel("toilet.obj");

    std::shared_ptr<Texture> grassTexture = textureManager.getTexture("grass.png");
	std::shared_ptr<Texture> woodenTexture = textureManager.getTexture("wooden_fence.png");
    std::shared_ptr<Texture> skyTexture = textureManager.getCubeMap();
    std::shared_ptr<Texture> zombieTexture = textureManager.getTexture("zombie.png");
    std::shared_ptr<Texture> toiletTexture = textureManager.getTexture("toilet.png");
	std::shared_ptr<Texture> skydomeTexture = textureManager.getTexture("skydome.png");

    Material* material1 = new Material(glm::vec3(0.1f), glm::vec3(0.1f), glm::vec3(0.1f), 1.0f, woodenTexture);
    Material* material2 = new Material(glm::vec3(0.1f), glm::vec3(0.1f), glm::vec3(0.1f), 1.0f, grassTexture);
    Material* skycubeM = new Material(glm::vec3(0.1f), glm::vec3(0.1f), glm::vec3(0.1f), 1.0f, skyTexture);
    Material* zombieM = new Material(glm::vec3(0.1f), glm::vec3(0.1f), glm::vec3(0.1f), 1.0f, zombieTexture);
    Material* toiletM = new Material(glm::vec3(0.1f), glm::vec3(0.1f), glm::vec3(0.1f), 1.0f, toiletTexture);
	Material* skydomeM = new Material(glm::vec3(0.1f), glm::vec3(0.1f), glm::vec3(0.1f), 1.0f, skydomeTexture);

    scenes[sceneIndex]->setSkyBox(new DrawableObject(skyCubeModel, vertexShaderSources[4], fragmentShaderSources[6], skycubeM));
    
    scenes[sceneIndex]->addObject(new DrawableObject(plainTextureModel, vertexShaderSources[3], fragmentShaderSources[3], material1, true));
    scenes[sceneIndex]->moveObject(0, 'l', 1.5f);

	scenes[sceneIndex]->addObject(new DrawableObject(plainTextureModel, vertexShaderSources[3], fragmentShaderSources[3], material2, true));
    scenes[sceneIndex]->moveObject(1, 'r', 1.5f);

	scenes[sceneIndex]->addObject(new DrawableObject(toilet, vertexShaderSources[3], fragmentShaderSources[3], toiletM, true));
    scenes[sceneIndex]->moveObject(2, 'u', 1.5f);
    
    scenes[sceneIndex]->addObject(new DrawableObject(zombie, vertexShaderSources[3], fragmentShaderSources[3], zombieM, true));
	scenes[sceneIndex]->moveObject(3, 'd', 1.5f);

	scenes[sceneIndex]->addLight(nullptr, glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(1.0f), 1.0f, LightType::DIRECTIONAL);
}

void Application::createSphereAtClick()
{
    int currObjIndex = scenes[currentScene]->getCurrObject();

    if (currObjIndex < 0 || currObjIndex >= scenes[currentScene]->objectsCount()) {
        cout << "Invalid object index." << endl;
        return;
    }

	glm::mat4 view = scenes[currentScene]->getViewMatrix();
	glm::mat4 projection = scenes[currentScene]->getProjectionMatrix(aspectRatio);

    // The viewport
    // glViewport(0, 0, width, height); was set, so viewport = [0,0,width,height]
    glm::vec4 viewport(0.0f, 0.0f, (float)width, (float)height);

    // Convert screen coordinates to NDC
    // We have lastClickX, lastClickY, lastClickDepth from handleClick
    glm::vec3 winCoords(lastClickX, (float)(lastClickY), lastClickDepth);

    // Unproject to get world coordinates
    glm::vec3 worldPos = glm::unProject(winCoords, view, projection, viewport);

    cout << "World position: " << worldPos.x << worldPos.y << worldPos.z << endl;

    // Create a sphere at worldPos
    std::shared_ptr<Model> sphereModel = modelManager.getModel("sphere", sphere, sizeof(sphere), 1);
    DrawableObject* newSphere = new DrawableObject(sphereModel, vertexShaderSources[2], fragmentShaderSources[3], true);
    newSphere->setPosition(worldPos);
	newSphere->moveObject(glm::vec3(0.0f, 0.2f, 0.0f));
    newSphere->setScale(0.2f);

    scenes[currentScene]->addObject(newSphere);
	scenes[currentScene]->registerNewObserver(newSphere);

    cout << "New sphere created at " << worldPos.x << ", " << worldPos.y << ", " << worldPos.z << endl;
}

void Application::removeSelectedObject()
{
    int currObjIndex = scenes[currentScene]->getCurrObject();

    if (currObjIndex < 0 || currObjIndex >= scenes[currentScene]->objectsCount()) {
        cout << "Invalid object index." << endl;
        return;
    }

    DrawableObject* objToDelete = scenes[currentScene]->getObject(currObjIndex);
    if (objToDelete == nullptr) {
        cout << "Object pointer is null." << endl;
        return;
    }
    
    if (objToDelete->getId() < 1){
		cout << "Cannot delete this object." << endl;
        return;
    }

    scenes[currentScene]->deleteObject(objToDelete);
	scenes[currentScene]->setCurrentObject(0);
    cout << "Object deleted." << endl;
}

void Application::addBallsDiffShaders(int sceneIndex) {
	std::shared_ptr<Model> sphereModel = modelManager.getModel("sphere", sphere, sizeof(sphere), 1);
    scenes[sceneIndex]->addLight(sphereModel, glm::vec3(0.1f), glm::vec3(1.0f, 0.5f, 0.0f), 1.0f, LightType::POINT);

    int numOfObjectInScene = scenes[sceneIndex]->objectsCount();
    scenes[sceneIndex]->addObject(new DrawableObject(sphereModel, vertexShaderSources[1], fragmentShaderSources[2], false));

	Material* material = new Material(glm::vec3(0.1f), glm::vec3(1.0f), glm::vec3(1.0f), 130.0f);

    for (int i = 3; i < 6; i++) {
        scenes[sceneIndex]->addObject(new DrawableObject(sphereModel, vertexShaderSources[2], fragmentShaderSources[i], false));
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
            if (scenes[currentScene]->getLightType(i) != LightType::SPOTLIGHT) {
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
        }

        // Increment and reset counter after 100 moves
        cntr++;
        if (cntr == 100) {
            cntr = 0;
        }
    }
}



