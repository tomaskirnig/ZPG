#include "Application.h"

// Vertex shader source code
const char* vertexShaderSource2 =
"#version 330\n"
"layout(location=0) in vec3 vp;"  // Position attribute
"layout(location=1) in vec3 vn;"  // Color attribute
"out vec3 color;"                 // Passing color to the fragment shader
"void main () {"
"   gl_Position = vec4 (vp, 1.0);"
"   color = vn;"                // Assigning input color to the output
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
"   gl_Position = projectionMatrix * viewMatrix * transformationMatrix * vec4(vp, 1.0);"  // Combine matrices
"   color = vn;"  // Pass color to the fragment shader
"}";

const char* vertexShaderSource4 =
"# version 400\n"
"layout(location = 0) in vec3 vp;" // position
"layout(location = 1) in vec3 vn;" // normal
"out vec4 worldPos;"
"out vec3 worldNorm;"
"uniform mat4 transformationMatrix;"
"uniform mat4 viewMatrix;"
"uniform mat4 projectionMatrix;"
"void main() {"
"   worldPos = transformationMatrix * vec4(vp, 1.0);"
"   mat4 normal = transformationMatrix; "          // problem - priste vysvetlime
"   worldNorm = vec3(normal * vec4(vn, 1.0));"
"   gl_Position = projectionMatrix * viewMatrix * transformationMatrix * vec4(vp, 1.0);" 
"}";


// Fragment shader source code
const char* fragmentShaderSource =
"#version 330\n"
"out vec4 frag_colour;"
"void main () {"
"     frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);"
//"     frag_colour = vec4 (0.5, 0.0, hello, 1.0);" // Shader compilation error test
"}";

const char* fragmentShaderSource2 =
"#version 330\n"
"in vec3 color;"
"out vec4 frag_colour;"
"void main () {"
"     frag_colour = vec4(color, 0.5);"  // Use the color passed from the vertex shader
"}";

const char* fragmentShaderSource3 =
"#version 330\n"
"in vec3 color;"     // Receive the interpolated color from the vertex shader
"out vec4 frag_colour;"  // Declare the output color variable
"void main() {"
"    frag_colour = vec4(color, 1.0);"  // Set the fragment color using the input color and alpha = 1.0
"}";

const char* fragmentShaderSource4 =
"#version 330 core\n"
"in vec3 ex_worldNormal;"
"in vec4 ex_worldPosition;"
"out vec4 out_Color;"
"uniform vec3 lightPosition;"
"uniform vec3 lightColor;"
"void main() {"
    "vec3 lightVector = normalize(lightPosition - vec3(ex_worldPosition));" // Calculate the light direction
    "float diff = max(dot(normalize(ex_worldNormal), lightVector), 0.0);"   // Calculate the diffuse lighting component
    "vec4 diffuse = vec4(diff * lightColor, 1.0);"
    "vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);"  // Add ambient lighting
    "out_Color = ambient + diffuse;"
"}";


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

    scenes.push_back(Scene());
	cameras.push_back(Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f));
	lights.push_back(Light(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));

    currentScene = 0;
	currentCamera = 0;
    
	lastX = width / 2;
	lastY = height / 2;
}

void Application::run() {
    float square[] = {
        0.5f,  0.5f, 0.0f,  0.5f,  0.5f, 1.0f,
        0.8f,  0.5f, 0.0f,  0.5f,  0.5f, 0.0f,
        0.5f,  0.8f, 0.0f,  0.0f,  0.5f, 0.0f,

        0.5f,  0.8f, 0.0f,  0.5f,  0.5f, 0.0f,
        0.8f,  0.5f, 0.0f,  0.0f,  0.0f, 1.0f,
        0.8f,  0.8f, 0.0f,  0.5f,  0.5f, 0.0f,
    };

    glEnable(GL_DEPTH_TEST);

	// Create scenens and camera
    scenes.push_back(Scene());
    scenes.push_back(Scene());
	cameras.push_back(Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f));

    // Adding objects to first scene
	scenes[0].addObject(new DrawableObject(tree, sizeof(tree), vertexShaderSource3, fragmentShaderSource3));
	scenes[0].addObject(new DrawableObject(bushes, sizeof(bushes), vertexShaderSource3, fragmentShaderSource3));
    scenes[0].addObject(new DrawableObject(square, sizeof(square), vertexShaderSource3, fragmentShaderSource3));

    currentObject = 0;

	// Add a forest to the second scene
	addForest(1, 50);

	addBalls(2);
    
    registerAllObservers();

    // Hide and lock cursor in app window
	centerCursor();
	disableAndLockCursor();

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processInput();

        if (currentScene == 2) {
            calculateLight();
        }

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
    
    cameras[currentCamera].notifyObservers(aspectRatio);

	cout << "Current scene: " << currentScene << endl;

    //calculateLight();
}

// Change the current scene to the previous one
void Application::currentSceneMinus() {
    currentScene--;
    if (currentScene < 0) {
        currentScene = scenes.size() - 1;
    }

    cameras[currentCamera].notifyObservers(aspectRatio);
    
	cout << "Current scene: " << currentScene << endl;

    //calculateLight();
}

// Change the current object to the next one
void Application::currentObjectPlus() {
	currentObject++;
	if (currentObject >= scenes[currentScene].objectsCount()) {
		currentObject = 0;
	}
	cout << "Current object: " << currentObject << endl;
}

void Application::currentObjectMinus() {
    currentObject--;
    if (currentObject < 0) {
        currentObject = scenes[currentScene].objectsCount() - 1;
    }
    cout << "Current object: " << currentObject << endl;
}

void Application::currentCameraPlus() {
	currentCamera++;
	if (currentCamera >= cameras.size()) {
		currentCamera = 0;
	}

    // Inicialize the camera (projection and view matrixes)
    cameras[currentCamera].notifyObservers(aspectRatio);

	cout << "Current camera: " << currentCamera << endl;

    calculateLight();
}

void Application::processInput() {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameras[currentCamera].ProcessKeyboardMovement('u', aspectRatio); // Move camera up
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameras[currentCamera].ProcessKeyboardMovement('d', aspectRatio); // Move camera down
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameras[currentCamera].ProcessKeyboardMovement('l', aspectRatio); // Move camera left
    }
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameras[currentCamera].ProcessKeyboardMovement('r', aspectRatio); // Move camera right
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        scenes[currentScene].moveObject(currentObject, 'u'); // Move object up
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        scenes[currentScene].moveObject(currentObject, 'd'); // Move object down
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        scenes[currentScene].moveObject(currentObject, 'l'); // Move object left
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        scenes[currentScene].moveObject(currentObject, 'r'); // Move object right
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        scenes[currentScene].moveObject(currentObject, 'b');  // Move object back
    }
    else if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        scenes[currentScene].moveObject(currentObject, 'f');  // Move object front
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

    //calculateLight();
}

void Application::registerAllObservers() {
    // Registering object shaders for cameras
    for (int i = 0; i < scenes.size(); i++) {
        vector<Shader*> shaders = scenes[i].getShaders();
        for (int j = 0; j < shaders.size(); j++) {
            for (int k = 0; k < 2; k++) {
                cameras[k].registerObserver((Observer*)shaders[j]);
            }
        }
    }

    // Inicialize the camera (projection and view matrixes)
    for (int i = 0; i < cameras.size(); i++) {
        cameras[i].notifyObservers(aspectRatio);
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
					app->scenes[app->currentScene].resetObjectRotation(app->currentObject); // Reset object rotation
					break;

				case GLFW_KEY_T:
					app->scenes[app->currentScene].resetObjectScale(app->currentObject); // Reset object scale

			    default:
				    break;
			}
            //app->calculateLight();
        }
    }
}

// Functions for generating objects in scenes
void Application::addForest(int sceneIndex, int numTrees) {
    random_device rd;
    mt19937 gen(rd());  // Random number generator

    // Trees
    // Range for random positions
    uniform_real_distribution<> disXTree(-10.0, 10.0);  // X-axis range 
    uniform_real_distribution<> disZTree(-20.0, 20.0);  // Z-axis range 

    // Random scaling
    uniform_real_distribution<> disScaleTree(0.4, 1.0);

    // Random Y-axis rotation (0 to 360 degrees in radians)
    uniform_real_distribution<> disRotationY(0.0, 360.0);

    // Place Trees
    for (int i = 0; i < numTrees; ++i) {
        // Generate random x, z positions 
        float randomX = disXTree(gen);
        float randomZ = disZTree(gen);
        float randomRotationY = glm::radians(disRotationY(gen));  // Random rotation in radians

        DrawableObject* treeObject = new DrawableObject(tree, sizeof(tree), vertexShaderSource3, fragmentShaderSource3);

        // Set the transformation matrix (position and scale) 
        Transformation* transform = treeObject->getTransformation();
        transform->setPosition(glm::vec3(randomX, -0.5, randomZ));  // Place at random x, y, z 
        transform->setScale(glm::vec3(disScaleTree(gen)));  // Apply random scaling
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), randomRotationY, glm::vec3(0.0f, 1.0f, 0.0f));  // Rotation matrix around Y-axis
        transform->setRotationMatrix(transform->getRotationMatrix() * rotationMatrix);  // Rotate around Y-axis

        // Add the tree to the specified scene
        scenes[sceneIndex].addObject(treeObject);
    }

	// Bushes
    uniform_real_distribution<> disXBush(-10.0, 10.0);  
    uniform_real_distribution<> disZBush(-20.0, 20.0);
    uniform_real_distribution<> disScaleBush(0.4, 1.0);

    for (int i = 0; i < numTrees; ++i) {

        float randomX = disXBush(gen);
        float randomZ = disZBush(gen);  
        float randomRotationY = glm::radians(disRotationY(gen));  

        DrawableObject* bushObject = new DrawableObject(bushes, sizeof(bushes), vertexShaderSource3, fragmentShaderSource3);

        Transformation* transform = bushObject->getTransformation();
        transform->setPosition(glm::vec3(randomX, -0.5, randomZ));
        transform->setScale(glm::vec3(disScaleBush(gen))); 
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), randomRotationY, glm::vec3(0.0f, 1.0f, 0.0f));  
        transform->setRotationMatrix(transform->getRotationMatrix() * rotationMatrix);

        scenes[sceneIndex].addObject(bushObject);
    }
}

void Application::addBalls(int sceneIndex) {
	int numOfObjectInScene = scenes[sceneIndex].objectsCount();
    for (int i = 0; i < 4; i++) {
        scenes[sceneIndex].addObject(new DrawableObject(sphere, sizeof(sphere), vertexShaderSource4, fragmentShaderSource4));
    }

    scenes[sceneIndex].moveObject(numOfObjectInScene++, 'u', 1.0);
    scenes[sceneIndex].moveObject(numOfObjectInScene++, 'd', 1.0);
    scenes[sceneIndex].moveObject(numOfObjectInScene++, 'l', 1.0);
    scenes[sceneIndex].moveObject(numOfObjectInScene++, 'r', 1.0);
}

void Application::calculateLight() {
	cout << "Calculating light" << endl;
	vector<Shader*> objShaders = scenes[currentScene].getShaders();
	for (int i = 0; i < lights.size(); i++) {
        for (int j = 0; j < objShaders.size(); j++) {
            lights[i].applyLighting(objShaders[j]);
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

        // Notify camera of the aspect ratio change
        app->cameras[app->currentCamera].notifyObservers(app->aspectRatio);
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
		app->cameras[app->currentCamera].ProcessMouseMovement(xOffset, yOffset, app->aspectRatio);
	    
        //app->calculateLight();
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
        app->cameras[app->currentCamera].ProcessMouseScroll(yOffset, app->aspectRatio);  // Pass the scroll input to your Application class
    }
}