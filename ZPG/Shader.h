#pragma once
#include <GL/glew.h>
#include <iostream>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "IObserver.h"
#include "ShaderLoader.h"

using namespace std;

class Shader : IObserver, ShaderLoader{
    private:
        GLuint ID;
        unordered_map<string, GLint> uniformLocations;  // Cache for uniform locations

        void checkCompileErrors(GLuint shader, string type);

    public:
        //Shader(const char* vertexSource, const char* fragmentSource);
        Shader(string vertexFile, string fragmentFile);

	    GLuint compileShader(const char* source, GLenum type);

        void use() {
            glUseProgram(ID);
        }

        // Observer's update method implementation
        void update(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, glm::vec3 lightPosition, glm::vec3 lightColor) override;

        void deleteShader() {
            glDeleteProgram(ID);
        }

        GLuint getShaderID() {
		    return ID;
	    }

        GLint getUniformLocation(const string& name);
};
