#pragma once
#include <GL/glew.h>

class ShaderProgram
{
private:
	GLuint ID;

public:
	ShaderProgram(const char* vertexPath, const char* fragmentPath);
	~ShaderProgram();

	void use();

};

