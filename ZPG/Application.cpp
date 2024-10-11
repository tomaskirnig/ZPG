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
	currentScene = 0;
}

Application::~Application() {
    // Clean up dynamically allocated memory
    for (auto shader : shaderPrograms) {
        delete shader;
    }
    for (auto model : models) {
        delete model;
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::addObject(const float* object, size_t size, const char* vertexSource, const char* fragmentSource) {
    shaderPrograms.push_back(new Shader(vertexSource, fragmentSource));
    models.push_back(new Model(object, size));
}

void Application::run() {
	// triangle
    const char* fragmentShaderSource =
        "#version 330\n"
        "out vec4 frag_colour;"
        "void main () {"
        "     frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);"
        //"     frag_colour = vec4 (0.5, 0.0, hello, 1.0);" // CHYBA SCHVALNE
        "}";

     float points[] = {
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
       -0.5f, -0.5f, 0.0f
    };

	// square
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

    float squareVertices[] = {
        0.5f,  0.5f, 0.0f,
        0.8f,  0.5f, 0.0f,
        0.8f,  0.8f, 0.0f,
        0.5f,  0.8f, 0.0f,
    };

    float a[] = {
        -.5f, -.5f, .5f,  0, 0, 1,
        -.5f, .5f, .5f,  0, 0, 1,
        .5f, .5f, .5f,  0, 0, 1,
        .5f, -.5f, .5f,  0, 0, 1 
    };

    glEnable(GL_DEPTH_TEST);

	// Create two scenes
	scenes.push_back(Scene());
	scenes.push_back(Scene());

	// Firs scene - tree, second scene - bushes
	scenes[0].addObject(new DrawableObject(tree, sizeof(tree), vertexShaderSource2, fragmentShaderSource2));
	scenes[1].addObject(new DrawableObject(bushes, sizeof(bushes), vertexShaderSource2, fragmentShaderSource2));

	//cout << "size of scenes: " << scenes.size() << endl;

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		scenes[currentScene].render();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}


void Application::currentScenePlus() {
    currentScene++;
    if (currentScene >= scenes.size()) {
        currentScene = 0;  // Wrap around to the first scene if we exceed the number of scenes
    }
}

void Application::currentSceneMinus() {
    currentScene--;
    if (currentScene < 0) {
        currentScene = scenes.size();
    }
}

void Application::error_callback(int error, const char* description) {
    fputs(description, stderr);
}

void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    cout << "key_callback [" << key << "," << scancode << "," << action << "," << mods << "]" << endl;

    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));

    if (app == NULL) {
        cerr << "Key_callback -> app = NULL"<< endl;
    }

    if (app) {
        if (action == GLFW_PRESS) { // Only change the scene when the key is pressed
            if (key == GLFW_KEY_RIGHT) {
                app->currentScenePlus();
				cout << "currentScene: " << app->currentScene << endl;
            }
            else if (key == GLFW_KEY_LEFT) {
                app->currentSceneMinus();
                cout << "currentScene: " << app->currentScene << endl;
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