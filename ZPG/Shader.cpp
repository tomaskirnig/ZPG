#include "Shader.h"


Shader::Shader(string vertexFile, string fragmentFile, float shininess) {
	this->shininess = shininess;

	cout << "Shininess" << this->shininess << endl;

    vertexFile = "../Shaders/Vertex/" + vertexFile + ".glsl";
	fragmentFile = "../Shaders/Fragment/" + fragmentFile + ".glsl";

	ID = loadShader(vertexFile.c_str(), fragmentFile.c_str());
}

// Compile a shader from source code
GLuint Shader::compileShader(const char* source, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    checkCompileErrors(shader, (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT");
    return shader;
}

// Check for compilation errors
void Shader::checkCompileErrors(GLuint shader, string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << endl;
            exit(EXIT_FAILURE);
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << endl;
            exit(EXIT_FAILURE);
        }
    }
}

// Updates the shader with the view and projection matrices + light properties
void Shader::update(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, glm::vec3 lightPosition, glm::vec3 lightColor, glm::vec3 viewPosition) {
    use();  // Activate the shader

    glUniformMatrix4fv(getUniformLocation("viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(getUniformLocation("projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    GLint tmpLoc = getUniformLocation("lightPosition");
    GLint tmpColorLoc = getUniformLocation("lightColor");

    if (tmpLoc != -1 && tmpColorLoc != -1) {
        glUniform3fv(tmpLoc, 1, glm::value_ptr(lightPosition));
        glUniform3fv(tmpColorLoc, 1, glm::value_ptr(lightColor));
    }

	GLuint shininesLoc = getUniformLocation("shininess");

	if (shininesLoc != -1) {
        glUniform1f(shininesLoc, shininess);
	}

	GLuint viewPosLoc = getUniformLocation("viewPosition");

	if (viewPosLoc != -1) {
		glUniform3fv(viewPosLoc, 1, glm::value_ptr(viewPosition));
	}
}

// Retrieve the location of a uniform, caching it after the first retrieval
GLint Shader::getUniformLocation(const string& name) {
    // If the uniform location is not in the cache, get it from the shader and cache it
    if (uniformLocations.find(name) == uniformLocations.end()) {
        uniformLocations[name] = glGetUniformLocation(ID, name.c_str());
    }
    // Return the cached location
    return uniformLocations[name];
}