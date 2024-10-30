#include "Shader.h"


Shader::Shader(const char* vertexSource, const char* fragmentSource) {
    GLuint vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
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
void Shader::update(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, glm::vec3 lightPosition, glm::vec3 lightColor) {
    use();  // Activate the shader

    glUniformMatrix4fv(getUniformLocation("viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(getUniformLocation("projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    GLint tmpLoc = getUniformLocation("lightPosition");
    GLint tmpColorLoc = getUniformLocation("lightColor");

    if (tmpLoc != -1 && tmpColorLoc != -1) {
        glUniform3fv(tmpLoc, 1, glm::value_ptr(lightPosition));
        glUniform3fv(tmpColorLoc, 1, glm::value_ptr(lightColor));
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