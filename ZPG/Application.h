#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>  
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
#include "TextureManager.h"
#include "Scene.h"

class Application
{
    private:
        GLFWwindow* window;
	    int width, height;
		int lastX, lastY;
        double lastClickX, lastClickY;
        float lastClickDepth;
		float aspectRatio;

        vector<Scene*> scenes;
		ModelManager modelManager;
        TextureManager textureManager;

        int currentScene;
        bool looking;

    public:
	    Application();
		~Application();

	    void run();
		float getAspectRatio();


		// Cursor controls
        void centerCursor();
		void disableAndLockCursor();
		void enableCursor();

		// Change the current scene
        void currentScenePlus();
		void currentSceneMinus();

        void currentObjectPlus();
		void currentObjectMinus();
		void setCurrentObject(int object);

        void currentCameraPlus();   

        void processInput();
		void handleClick(double x, double y);
        void selectObject(GLuint id);

        void registerAllObservers();

		void addScene();

        void moveLightsRandom();
         
		// Object generation
		void addForest(int sceneIndex, int numTrees);
        void addBalls(int sceneIndex);
        void addBallsDiffShaders(int sceneIndex);
		void addTextures(int sceneIndex);

        void createSphereAtClick();
        void removeSelectedObject();

        // Callback functions as static methods
        static void error_callback(int error, const char* description);
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void window_focus_callback(GLFWwindow* window, int focused);
        static void window_iconify_callback(GLFWwindow* window, int iconified);
        static void window_size_callback(GLFWwindow* window, int width, int height);
        static void cursor_callback(GLFWwindow* window, double x, double y);
        static void button_callback(GLFWwindow* window, int button, int action, int mode);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
        static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
};

