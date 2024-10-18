#include "Application.h"

// Vertex shader source code
const char* vertexShaderSource2 =
"#version 330\n"
"layout(location=0) in vec3 vp;"  // Position attribute
"layout(location=1) in vec3 vn;"  // Color attribute
"out vec3 color;"                 // Passing color to the fragment shader
"void main () {"
"     gl_Position = vec4 (vp, 1.0);"
"     color = vn;"                // Assigning input color to the output
"}";

const char* vertexShaderSource3 =
"#version 330\n"
"layout(location=0) in vec3 vp;"  // Vertex position
"layout(location=1) in vec3 vn;"  // Vertex normal or color
"uniform mat4 transformationMatrix;"  // Transformation matrix
"uniform mat4 viewMatrix;"            // View matrix (from the camera)
"uniform mat4 projectionMatrix;"      // Projection matrix (perspective or orthographic)
"out vec3 color;"                     // Passing color to the fragment shader
"void main() {"
"    gl_Position = projectionMatrix * viewMatrix * transformationMatrix * vec4(vp, 1.0);"  // Combine matrices
"    color = vn;"  // Pass color to the fragment shader
"}";

// Fragment shader source code
const char* fragmentShaderSource =
"#version 330\n"
"out vec4 frag_colour;"
"void main () {"
"     frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);"
//"     frag_colour = vec4 (0.5, 0.0, hello, 1.0);" // CHYBA SCHVALNE
"}";

const char* fragmentShaderSource2 =
"#version 330\n"
"in vec3 color;"
"out vec4 frag_colour;"
"void main () {"
"     frag_colour = vec4(color, 1.0);"  // Use the color passed from the vertex shader
"}";

const char* fragmentShaderSource3 =
"#version 330\n"
"in vec3 color;"     // Receive the interpolated color from the vertex shader
"out vec4 frag_colour;"  // Declare the output color variable
"void main() {"
"    frag_colour = vec4(color, 1.0);"  // Set the fragment color using the input color and alpha = 1.0
"}";


Application::Application() {
    if (!glfwInit()) {
        cerr << "Failed to initialize GLFW" << endl;
        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(1536, 864, "ZPG", NULL, NULL);
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

    glfwSetWindowUserPointer(window, this);

    glfwGetFramebufferSize(window, &width, &height);
    aspectRatio = width / (float)height;
    glViewport(0, 0, width, height);	

    scenes.push_back(Scene());
	cameras.push_back(Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f));
	
    currentScene = 0;
	currentCamera = 0;
    
	lastX = width / 2;
	lastY = height / 2;
}

void Application::run() {
    float deltaTime = 0.0f; // Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame

    float square[] = {
        0.5f,  0.5f, 0.0f,  0, 0, 1,
        0.8f,  0.5f, 0.0f,  0, 0, 1,
        0.5f,  0.8f, 0.0f,  0, 0, 1,

        0.5f,  0.8f, 0.0f,  0, 0, 1,
        0.8f,  0.5f, 0.0f,  0, 0, 1,
        0.8f,  0.8f, 0.0f,  0, 0, 1
    };

    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Create second scene
	scenes.push_back(Scene());
	cameras.push_back(Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f));

    // Adding objects to scnene
	scenes[0].addObject(new DrawableObject(tree, sizeof(tree), vertexShaderSource3, fragmentShaderSource3));
	scenes[0].addObject(new DrawableObject(bushes, sizeof(bushes), vertexShaderSource3, fragmentShaderSource3));
    scenes[0].addObject(new DrawableObject(square, sizeof(square), vertexShaderSource3, fragmentShaderSource));


    for (int i = 0; i < scenes.size(); i++) {
		vector<Shader*> shaders = scenes[i].getShaders();
        for (int j = 0; j < shaders.size(); j++) {
			cameras[0].registerObserver((Observer*)shaders[j]);
        }
	}

    // After registering observers
	for (int i = 0; i < cameras.size(); i++) {
		cameras[i].notifyObservers(aspectRatio);
	}

    currentObject = 0;

	// Add a forest to the second scene
	addForest(1, 50);

    float currentFrame;
    
	centerCursor();
	disableAndLockCursor();

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processInput();

		scenes[currentScene].render();

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
}

// Change the current scene to the previous one
void Application::currentSceneMinus() {
    currentScene--;
    if (currentScene < 0) {
        currentScene = scenes.size() - 1;
    }
}

// Change the current object to the next one
void Application::currentObjectPlus() {
	currentObject++;
	if (currentObject >= scenes[currentScene].objectsCount()) {
		currentObject = 0;
	}
	cout << "Current object: " << currentObject << endl;
}

void Application::currentCameraPlus() {
	currentCamera++;
	if (currentCamera >= cameras.size()) {
		currentCamera = 0;
	}
	cout << "Current camera: " << currentCamera << endl;
}

void Application::error_callback(int error, const char* description) {
    fputs(description, stderr);
}

void Application::processInput() {
    // Get the current state of control keys and move the object 
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        //scenes[currentScene].moveObject(currentObject, 'u');  // Move up
		cameras[currentCamera].ProcessKeyboardMovement('w', aspectRatio);
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        //scenes[currentScene].moveObject(currentObject, 'd');  // Move down
		cameras[currentCamera].ProcessKeyboardMovement('s', aspectRatio);
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        //scenes[currentScene].moveObject(currentObject, 'l');  // Move left
		cameras[currentCamera].ProcessKeyboardMovement('a', aspectRatio);
    }
	else if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        //scenes[currentScene].moveObject(currentObject, 'r');  // Move right
		cameras[currentCamera].ProcessKeyboardMovement('d', aspectRatio);
    }
    else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        scenes[currentScene].moveObject(currentObject, 'b');  // Move back
    }
    else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        scenes[currentScene].moveObject(currentObject, 'f');  // Move front
    }
	else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		scenes[currentScene].rotateObject(currentObject, 1);  // Rotate around x-axis
	}
	else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		scenes[currentScene].rotateObject(currentObject, 2);  // Rotate around x-axis
	}
	else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		scenes[currentScene].rotateObject(currentObject, 3);  // Rotate around y-axis
	}
	else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
		scenes[currentScene].rotateObject(currentObject, 4);  // Rotate around y-axis
	}
	else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
		scenes[currentScene].rotateObject(currentObject, 5);  // Rotate around z-axis
	}
	else if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
		scenes[currentScene].rotateObject(currentObject, 6);  // Rotate around z-axis
	}
    else if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
        scenes[currentScene].scaleObject(currentObject, 'u');  // Scale up
    }
    else if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
        scenes[currentScene].scaleObject(currentObject, 'd');  // Scale down
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
	// Change of scenes, objects and reset of object rotation and scale
    if (app) {
        if (action == GLFW_PRESS) {
            switch (key)
            {
                case GLFW_KEY_RIGHT:
                    app->currentScenePlus();
                    break;

                case GLFW_KEY_LEFT:
                    app->currentSceneMinus();
                    break;

				case GLFW_KEY_TAB:
					//app->currentObjectPlus();
					app->currentCameraPlus();
					break;

				case GLFW_KEY_R:
					app->scenes[app->currentScene].resetObjectRotation(app->currentObject);
					break;

				case GLFW_KEY_T:
					app->scenes[app->currentScene].resetObjectScale(app->currentObject);

			    default:
				    break;
			}
        }
    }
}

void Application::addForest(int sceneIndex, int numTrees) {
    std::random_device rd;
    std::mt19937 gen(rd());  // Random number generator

    // Trees
    // Range for random positions
    std::uniform_real_distribution<> disXTree(-10.0, 10.0);  // X-axis range 
    //std::uniform_real_distribution<> disYTree(-1.0, -0.5);  // Y-axis range
    std::uniform_real_distribution<> disZTree(-20.0, 20.0);  // Z-axis range 

    // Random scaling
    std::uniform_real_distribution<> disScaleTree(0.4, 1.0);

    // Random Y-axis rotation (0 to 360 degrees in radians)
    std::uniform_real_distribution<> disRotationY(0.0, 360.0);

    // Place Trees
    for (int i = 0; i < numTrees; ++i) {
        // Generate random x, y, z positions 
        float randomX = disXTree(gen);
        float randomY = -0.5; //disYTree(gen);
        float randomZ = disZTree(gen);
        float randomRotationY = glm::radians(disRotationY(gen));  // Random rotation in radians

        // Create a new DrawableObject
        DrawableObject* treeObject = new DrawableObject(tree, sizeof(tree), vertexShaderSource3, fragmentShaderSource3);

        // Set the transformation matrix (position and scale) 
        Transformation* transform = treeObject->getTransformation();
        transform->setPosition(glm::vec3(randomX, randomY, randomZ));  // Place at random x, y, z 
        transform->setScale(glm::vec3(disScaleTree(gen)));  // Apply random scaling
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), randomRotationY, glm::vec3(0.0f, 1.0f, 0.0f));  // Rotation matrix around Y-axis
        transform->applyRotationMatrix(rotationMatrix);  // Rotate around Y-axis

        // Add the tree to the specified scene
        scenes[sceneIndex].addObject(treeObject);
    }

	// Bushes
    std::uniform_real_distribution<> disXBush(-10.0, 10.0);  
    //std::uniform_real_distribution<> disYBush(-0.8, -0.3); 
    std::uniform_real_distribution<> disZBush(-20.0, 20.0);

    std::uniform_real_distribution<> disScaleBush(0.4, 1.0);

    for (int i = 0; i < numTrees; ++i) {

        float randomX = disXBush(gen);
        float randomY = -0.5; //disYBush(gen);
        float randomZ = disZBush(gen);  

        DrawableObject* bushObject = new DrawableObject(bushes, sizeof(bushes), vertexShaderSource3, fragmentShaderSource3);

        Transformation* transform = bushObject->getTransformation();
        transform->setPosition(glm::vec3(randomX, randomY, randomZ));  
        transform->setScale(glm::vec3(disScaleBush(gen))); 

        scenes[sceneIndex].addObject(bushObject);
    }

	for (int i = 0; i < scenes.size(); i++) {
		vector<Shader*> shaders = scenes[i].getShaders();
		for (int j = 0; j < shaders.size(); j++) {
			cameras[0].registerObserver((Observer*)shaders[j]);
		}
	}
}



void Application::window_focus_callback(GLFWwindow* window, int focused) { cout << "window_focus_callback" << endl; }

void Application::window_iconify_callback(GLFWwindow* window, int iconified) { cout << "window_iconify_callback" << endl;
}
void Application::window_size_callback(GLFWwindow* window, int width, int height) {
    //cout << "resize " << width << ", " << height << endl;
    glViewport(0, 0, width, height);
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));

	if (app == NULL) {
		cerr << "Window_size_callback -> app = NULL" << endl;
	}

	if (app) {
		app->width = width;
		app->height = height;
		app->aspectRatio = width / (float)height;

        // Notify camera of the aspect ratio change
        app->cameras[app->currentCamera].notifyObservers(app->aspectRatio);
    }
}

void Application::cursor_callback(GLFWwindow* window, double x, double y) { 
    cout << "cursor_callback" << "x: " << x << " y: " << y << endl;
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));

    if (app == NULL) {
        cerr << "Cursor_callback -> app = NULL" << endl;
    }
	if (app) {
        double xOffset = x - app->lastX;
        double yOffset =  app->lastY - y;

        app->lastX = x;
        app->lastY = y;

		app->cameras[app->currentCamera].ProcessMouseMovement(xOffset, yOffset, true, app->aspectRatio);
	}
}

void Application::button_callback(GLFWwindow* window, int button, int action, int mode) {
    if (action == GLFW_PRESS) {
		cout << "button_callback [" << button << "," << action << "," << mode << "]" << endl;
	}
}