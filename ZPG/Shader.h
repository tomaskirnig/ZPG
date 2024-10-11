#pragma once
#include <GL/glew.h>
#include <iostream>

using namespace std;

class Shader {
    private:
        GLuint ID;

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
};
