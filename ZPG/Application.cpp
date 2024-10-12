#include "Application.h"



const char* vertexShaderSource =
"#version 330\n"
"layout(location=0) in vec3 vp;"
"void main () {"
"     gl_Position = vec4 (vp, 1.0);"
"}";

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
"layout(location=0) in vec3 vp;"  
"layout(location=1) in vec3 vn;" 
"uniform mat4 transformationMatrix;"  // Add transformation matrix uniform
"out vec3 color;"                 // Passing color to the fragment shader
"void main () {"
"     gl_Position = transformationMatrix * vec4(vp, 1.0);"  // Apply transformation matrix to position
"     color = vn;"                // Assigning input color to the output
"}";


Application::Application() {
    if (!glfwInit()) {
        cerr << "Failed to initialize GLFW" << endl;
        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(800, 600, "ZPG", NULL, NULL);
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
    //glfwSetFramebufferSizeCallback(window, window_size_callback);
	glfwSetKeyCallback(window, key_callback);
    /*glfwSetWindowFocusCallback(window, window_focus_callback);
	glfwSetWindowIconifyCallback(window, window_iconify_callback);
	glfwSetCursorPosCallback(window, cursor_callback);
	glfwSetMouseButtonCallback(window, button_callback);*/

    glfwSetWindowUserPointer(window, this);

    glfwGetFramebufferSize(window, &width, &height);
    float ratio = width / (float)height;
    glViewport(0, 0, width, height);	
    scenes.push_back(Scene());
	currentScene = 0;
}

//Application::~Application() {
//    // Clean up dynamically allocated memory
//    for (auto shader : shaderPrograms) {
//        delete shader;
//    }
//    for (auto model : models) {
//        delete model;
//    }
//
//    glfwDestroyWindow(window);
//    glfwTerminate();
//}


void Application::run() {
    const char* fragmentShaderSource =
        "#version 330\n"
        "out vec4 frag_colour;"
        "void main () {"
        "     frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);"
        //"     frag_colour = vec4 (0.5, 0.0, hello, 1.0);" // CHYBA SCHVALNE
        "}";

    /*const char* fragmentShaderSource2 =
        "#version 330\n"
        "out vec4 frag_colour;"
        "void main () {"
        "     frag_colour = vec4 (0.0, 0.5, 0.5, 1.0);"
        "}";*/

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


    float square[] = {
        0.5f,  0.5f, 0.0f,  0, 0, 1,
        0.8f,  0.5f, 0.0f,  0, 0, 1,
        0.5f,  0.8f, 0.0f,  0, 0, 1,

        0.5f,  0.8f, 0.0f,  0, 0, 1,
        0.8f,  0.5f, 0.0f,  0, 0, 1,
        0.8f,  0.8f, 0.0f,  0, 0, 1
    };

    glEnable(GL_DEPTH_TEST);

	// Create second scene
	scenes.push_back(Scene());

    // Adding objects to scnenes
	scenes[0].addObject(new DrawableObject(tree, sizeof(tree), vertexShaderSource3, fragmentShaderSource3));
	scenes[0].addObject(new DrawableObject(bushes, sizeof(bushes), vertexShaderSource3, fragmentShaderSource3));
    scenes[0].addObject(new DrawableObject(square, sizeof(square), vertexShaderSource3, fragmentShaderSource));
	scenes[1].addObject(new DrawableObject(bushes, sizeof(bushes), vertexShaderSource3, fragmentShaderSource3));

    scenes[0].moveObject(currentObject, 'd');
    scenes[0].moveObject(currentObject, 'd');

    currentObject = 0;
	//cout << "size of scenes: " << scenes.size() << endl;

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processInput();
		scenes[currentScene].render();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
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

void Application::error_callback(int error, const char* description) {
    fputs(description, stderr);
}

void Application::processInput() {
    // Get the current state of control keys and move the object 
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        scenes[currentScene].moveObject(currentObject, 'u');  // Move up
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        scenes[currentScene].moveObject(currentObject, 'd');  // Move down
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        scenes[currentScene].moveObject(currentObject, 'l');  // Move left
    }
	else if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        scenes[currentScene].moveObject(currentObject, 'r');  // Move right
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
                    app->currentScenePlus();
                    break;

                case GLFW_KEY_LEFT:
                    app->currentSceneMinus();
                    break;

				case GLFW_KEY_TAB:
					app->currentObjectPlus();
					break;

				case GLFW_KEY_R:
					app->scenes[app->currentScene].resetObjectRotation(app->currentObject);
					break;

			    default:
				    break;
			}
        }
    }
}

void Application::window_focus_callback(GLFWwindow* window, int focused) { cout << "window_focus_callback" << endl; }

void Application::window_iconify_callback(GLFWwindow* window, int iconified) { cout << "window_iconify_callback" << endl;
}
void Application::window_size_callback(GLFWwindow* window, int width, int height) {
    cout << "resize " << width << ", " << height << endl;
    glViewport(0, 0, width, height);
}

void Application::cursor_callback(GLFWwindow* window, double x, double y) { cout << "cursor_callback" << endl; }

void Application::button_callback(GLFWwindow* window, int button, int action, int mode) {
    if (action == GLFW_PRESS) {
		cout << "button_callback [" << button << "," << action << "," << mode << "]" << endl;
	}
}