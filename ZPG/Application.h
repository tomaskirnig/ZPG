#pragma once

//Include GLEW
#include <GL/glew.h>

//Include GLFW  
#include <GLFW/glfw3.h>  

//Include GLM  
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

//Standard libraries
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>

//Include custom classes
#include "Shader.h"
#include "Model.h"
#include "Scene.h"

//Include objects
#include "sphere.h"
#include "tree.h"
#include "bushes.h"

using namespace std;

class Application
{
    private:
        GLFWwindow* window;
	    int width, height;
        vector<Shader*> shaderPrograms;
        vector<Model*> models;
		vector<Scene> scenes;
		int currentScene;

    public:
	    Application();
	    ~Application();
	    void addObject(const float* object, size_t size, const char* vertexSource, const char* fragmentSource);
	    void run();

		// Change the current scene
        void currentScenePlus();
		void currentSceneMinus();

        // Callback functions as static methods
        static void error_callback(int error, const char* description);
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void window_focus_callback(GLFWwindow* window, int focused);
        static void window_iconify_callback(GLFWwindow* window, int iconified);
        static void window_size_callback(GLFWwindow* window, int width, int height);
        static void cursor_callback(GLFWwindow* window, double x, double y);
        static void button_callback(GLFWwindow* window, int button, int action, int mode);

};

