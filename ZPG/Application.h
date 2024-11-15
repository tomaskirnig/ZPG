#pragma once

//Include GLEW
#include <GL/glew.h>

//Include GLFW  
#include <GLFW/glfw3.h>  

//Include GLM  
#include <glm/vec3.hpp> 
#include <glm/vec4.hpp> 
#include <glm/mat4x4.hpp> 
#include <glm/gtc/type_ptr.hpp> 

//Standard libraries
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <random>  

//Include custom classes
#include "ModelManager.h"
#include "Scene.h"
//#include "Camera.h"
//#include "Light.h"
//#include "Shader.h"


using namespace std;

class Application
{
    private:
        GLFWwindow* window;
	    int width, height;
		int lastX, lastY;
		float aspectRatio;

        vector<Scene*> scenes;
		ModelManager modelManager;

        int currentScene;


    public:
	    Application();
		~Application();

	    void run();
		float getAspectRatio();

        void loadModels();

		// Cursor controls
        void centerCursor();
		void disableAndLockCursor();
		void enableCursor();

		// Change the current scene
        void currentScenePlus();
		void currentSceneMinus();

        void currentObjectPlus();
		void currentObjectMinus();

        void currentCameraPlus();   

        void processInput();

        void registerAllObservers();

		void addScene();

        void moveLightsRandom();
         
		// Object generation
		void addForest(int sceneIndex, int numTrees);
        void addBalls(int sceneIndex);
        void addBallsDiffShaders(int sceneIndex);
        void addMonkeys(int sceneIndex);

        // Callback functions as static methods
        static void error_callback(int error, const char* description);
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void window_focus_callback(GLFWwindow* window, int focused);
        static void window_iconify_callback(GLFWwindow* window, int iconified);
        static void window_size_callback(GLFWwindow* window, int width, int height);
        static void cursor_callback(GLFWwindow* window, double x, double y);
        static void button_callback(GLFWwindow* window, int button, int action, int mode);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

};

