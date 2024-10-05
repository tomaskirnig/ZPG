#pragma once
#include <GL/glew.h>
#include <iostream>


class Shader {
public:
    GLuint ID;
    Shader(const char* vertexSource, const char* fragmentSource);

    void use() {
        glUseProgram(ID);
		//cout << "Shader in use" << endl;
    }

    void deleteShader() {
        glDeleteProgram(ID);
    }

private:
    void checkCompileErrors(GLuint shader, std::string type);
};
