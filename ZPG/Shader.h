#pragma once
#include <GL/glew.h>
#include <iostream>


class Shader {
private:
    GLuint ID;

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

private:
    void checkCompileErrors(GLuint shader, std::string type);
};
