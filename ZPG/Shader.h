#pragma once
#include <GL/glew.h>
#include <iostream>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Observer.h"

using namespace std;

class Shader : Observer {
    private:
        GLuint ID;
        unordered_map<string, GLint> uniformLocations;  // Cache for uniform locations

        void checkCompileErrors(GLuint shader, string type);

    public:
        Shader(const char* vertexSource, const char* fragmentSource);
	    GLuint compileShader(const char* source, GLenum type);

        void use() {
            glUseProgram(ID);
        }

        // Observer's update method implementation
        void update(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override {
            use();
            glUniformMatrix4fv(getUniformLocation("viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
            glUniformMatrix4fv(getUniformLocation("projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        }

        void deleteShader() {
            glDeleteProgram(ID);
        }

        GLuint getShaderID() {
		    return ID;
	    }

        GLint getUniformLocation(const string& name);
};
