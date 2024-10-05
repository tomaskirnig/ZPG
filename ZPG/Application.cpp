#include "Application.h"


void Application::error_callback(int error, const char* description) {
    fputs(description, stderr);
}

void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    cout << "key_callback [" << key << "," << scancode << "," << action << "," << mods << "]" << endl;
}

void Application::window_focus_callback(GLFWwindow* window, int focused) { cout << "window_focus_callback" << endl; }
void Application::window_iconify_callback(GLFWwindow* window, int iconified) { cout << "window_iconify_callback" << endl;  }
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