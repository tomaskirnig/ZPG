#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Shader.h"

class Light {
private:
    glm::vec3 lightPosition;
    glm::vec3 lightColor;

public:
    Light(glm::vec3 position, glm::vec3 color): lightPosition(position), lightColor(color) {}

    // Set/Get light properties
    void setPosition(const glm::vec3& position) { lightPosition = position; }
    void setColor(const glm::vec3& color) { lightColor = color; }

	glm::vec3 getPosition() { return lightPosition; }
    glm::vec3 getColor() { return lightColor; }

    // Apply the light properties to the shader
    //void applyLighting(Shader* shader) {
    //    shader->use();  // Activate the shader

    //    // Send the light's position and color to the shader
    //    glUniform3fv(shader->getUniformLocation("lightPosition"), 1, glm::value_ptr(lightPosition));
    //    glUniform3fv(shader->getUniformLocation("lightColor"), 1, glm::value_ptr(lightColor));
    //}
};
