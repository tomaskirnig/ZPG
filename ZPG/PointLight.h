#pragma once
#include "Light.h"

class PointLight : public Light {
public:
    PointLight(std::shared_ptr<Model> model, const glm::vec3& position, const glm::vec3& color, float intensity,
        const std::string& vertexShaderFile = "vertexShaderSource3",
        const std::string& fragmentShaderFile = "LambertFragmentShaderSource",
        Material* material = new Material())
        : Light(model, vertexShaderFile, fragmentShaderFile, color, intensity, material, LightType::POINT) {
        setPosition(position);
    }

	glm::vec3 getDirection() const override { return glm::vec3(0.0f); }
	void setDirection(const glm::vec3& dir) override { /* Do nothing */ }
};
