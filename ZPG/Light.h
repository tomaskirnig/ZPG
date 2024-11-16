#pragma once
#include <glm/glm.hpp>
#include "DrawableObject.h"
#include "LightType.h"

class Light : public DrawableObject {
protected:
    LightType type;
    glm::vec3 lightColor;
    float intensity;

public:
    Light(std::shared_ptr<Model> model,
        const std::string& vertexShaderFile = "vertexShaderSource3", 
        const std::string& fragmentShaderFile = "LambertFragmentShaderSource",
        const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f), 
        float intensity = 1.0f,
        float shininess = 1.0f,
        LightType type = LightType::POINT)
        : DrawableObject(model, vertexShaderFile, fragmentShaderFile, shininess),
        lightColor(color), intensity(intensity), type(type) {
		this->setScale(0.1f);
    }

    // Set/Get light properties
    void setColor(const glm::vec3& color) { lightColor = color; }
    glm::vec3 getColor() const { return lightColor; }

    void setIntensity(float intensity) { this->intensity = intensity; }
    float getIntensity() const { return intensity; }
    
    LightType getType() const { return type; }

    virtual glm::vec3 getDirection() const = 0;
    virtual void setDirection(const glm::vec3& direction) = 0;

    virtual float getCutOff() const { return 0.0f; }
    virtual float getOuterCutOff() const { return 0.0f; }
};
