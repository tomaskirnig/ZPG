#include "Application.h"


const char* vertexShaderSource =
"#version 330\n"
"layout(location=0) in vec3 vp;"
"void main () {"
"     gl_Position = vec4 (vp, 1.0);"
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
    glfwSetFramebufferSizeCallback(window, window_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetWindowFocusCallback(window, window_focus_callback);
	glfwSetWindowIconifyCallback(window, window_iconify_callback);
	glfwSetCursorPosCallback(window, cursor_callback);
	glfwSetMouseButtonCallback(window, button_callback);


    glfwGetFramebufferSize(window, &width, &height);
    float ratio = width / (float)height;
    glViewport(0, 0, width, height);	
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
    const char* fragmentShaderSource2 =
        "#version 330\n"
        "out vec4 frag_colour;"
        "void main () {"
        "     frag_colour = vec4 (0.0, 0.5, 0.5, 1.0);"
        "}";

    float squareVertices[] = {
        0.5f,  0.5f, 0.0f,
        0.8f,  0.5f, 0.0f,
        0.8f,  0.8f, 0.0f,
        0.5f,  0.8f, 0.0f,
    };

    shaderPrograms.push_back( new Shader(vertexShaderSource, fragmentShaderSource));
    models.push_back(new Model(points, sizeof(points)));

	shaderPrograms.push_back(new Shader(vertexShaderSource, fragmentShaderSource2));
	models.push_back(new Model(squareVertices, sizeof(squareVertices)));

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderPrograms[0]->use();
        models[0]->draw();

		shaderPrograms[1]->use();
		models[1]->draw();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void Application::error_callback(int error, const char* description) {
    fputs(description, stderr);
}

void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    cout << "key_callback [" << key << "," << scancode << "," << action << "," << mods << "]" << endl;
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