#pragma once
#include <glm/glm.hpp>
#include "DrawableObject.h"

class Light : public DrawableObject {
private:
    glm::vec3 lightColor;
    float intensity;

public:
    Light(const float* points = sphere, 
        size_t sizeOfPoints = sizeof(sphere), 
        const std::string& vertexShaderFile = "vertexShaderSource3", 
        const std::string& fragmentShaderFile = "LambertFragmentShaderSource",
        const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f), 
        float intensity = 1.0f,
        float shininess = 1.0f)
        : DrawableObject(points, sizeOfPoints, vertexShaderFile, fragmentShaderFile, shininess),
        lightColor(color), intensity(intensity) {
		this->setScale(0.1f);
    }

    // Set/Get light properties
    void setColor(const glm::vec3& color) { lightColor = color; }
    glm::vec3 getColor() const { return lightColor; }

    void setIntensity(float intensity) { this->intensity = intensity; }
    float getIntensity() const { return intensity; }
};
