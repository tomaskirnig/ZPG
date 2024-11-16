#pragma once
#include "Light.h"

class SpotLight : public Light {
private:
    glm::vec3 direction;
    float cutOff;
    float outerCutOff;

public:
    SpotLight(std::shared_ptr<Model> model, const glm::vec3& position, const glm::vec3& direction,
        const glm::vec3& color, float intensity, float cutOffAngle, float outerCutOffAngle,
        const std::string& vertexShaderFile = "vertexShaderSource3",
        const std::string& fragmentShaderFile = "LambertFragmentShaderSource",
        float shininess = 1.0f)
        : Light(model, vertexShaderFile, fragmentShaderFile, color, intensity, shininess, LightType::SPOTLIGHT),
        direction(glm::normalize(direction)), cutOff(cutOffAngle), outerCutOff(outerCutOffAngle) {
        setPosition(position);
    }

    glm::vec3 getDirection() const override { return direction; }
    void setDirection(const glm::vec3& dir) override { direction = glm::normalize(dir); }

    float getCutOff() const { return cutOff; }
    void setCutOff(float angle) { cutOff = angle; }

    float getOuterCutOff() const { return outerCutOff; }
    void setOuterCutOff(float angle) { outerCutOff = angle; }
};
