#include "ShaderProgram.h"
#include "Shader.h"

ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragmentPath)
{
	Shader* shader = new Shader(vertexPath, fragmentPath);
	ID = shader->getShaderID();
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(ID);
}

void ShaderProgram::use()
{
	glUseProgram(ID);
}