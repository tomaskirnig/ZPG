#pragma once
#include "Light.h"

class DirectionalLight : public Light {
private:
    glm::vec3 direction;

public:
    DirectionalLight(const glm::vec3& direction, const glm::vec3& color, float intensity)
        : Light(nullptr,"", "", color, intensity, new Material(), LightType::DIRECTIONAL), direction(glm::normalize(direction)) {
        // Directional lights do not have a position or model 
    }

    glm::vec3 getDirection() const override { return direction; }
    void setDirection(const glm::vec3& dir) override { direction = glm::normalize(dir); }
};
