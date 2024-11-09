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
// Shader.cpp
#include "Shader.h"

void Shader::update(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const std::vector<LightData>& lights, const glm::vec3& viewPosition) {
    // Set the shader program
    use();

    // Send the view and projection matrices to the shader
    glUniformMatrix4fv(getUniformLocation("viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(getUniformLocation("projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Send the view position
    glUniform3fv(getUniformLocation("viewPosition"), 1, glm::value_ptr(viewPosition));

    // Send the number of lights
    glUniform1i(getUniformLocation("numberOfLights"), std::min(MAX_LIGHTS, (int)lights.size()));

    // Send the light data
    for (size_t i = 0; i < std::min(MAX_LIGHTS, (int)lights.size()); ++i) {
        std::string index = std::to_string(i);
        glUniform3fv(getUniformLocation("lights[" + index + "].position"), 1, glm::value_ptr(lights[i].position));
        glUniform3fv(getUniformLocation("lights[" + index + "].color"), 1, glm::value_ptr(lights[i].color));
        glUniform1f(getUniformLocation("lights[" + index + "].intensity"), lights[i].intensity);
    }

    // Set the shininess
    glUniform1f(getUniformLocation("shininess"), shininess);
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