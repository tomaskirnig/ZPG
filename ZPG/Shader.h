#pragma once
#include <GL/glew.h>
#include <iostream>
#include <unordered_map>

using namespace std;

class Shader {
    private:
        GLuint ID;
        unordered_map<string, GLint> uniformLocations;  // Cache for uniform locations

        void checkCompileErrors(GLuint shader, std::string type);

    public:
        Shader(const char* vertexSource, const char* fragmentSource);
	    GLuint compileShader(const char* source, GLenum type);
        void use() {
            glUseProgram(ID);
        }

        void deleteShader() {
            glDeleteProgram(ID);
        }

        GLuint getShaderID() {
		    return ID;
	    }

        // Retrieve the location of a uniform, caching it after the first retrieval
        GLint getUniformLocation(const string& name) {
            // If the uniform location is not in the cache, get it from the shader and cache it
            if (uniformLocations.find(name) == uniformLocations.end()) {
                uniformLocations[name] = glGetUniformLocation(ID, name.c_str());
            }
            // Return the cached location
            return uniformLocations[name];
        }
};
