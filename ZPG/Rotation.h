#pragma once

#include "BaseTransformation.h"
#include <glm/gtc/matrix_transform.hpp>

class Rotation : public BaseTransformation {
public:
    float angle;
    glm::vec3 axis;

    Rotation(float angleDegrees, const glm::vec3& axis) : angle(angleDegrees), axis(axis) {}

    glm::mat4 getMatrix() const override {
        if (glm::length(axis) == 0.0f) {
            return glm::mat4(1.0f); // Return identity matrix if axis is zero
        }
        return glm::rotate(glm::mat4(1.0f), angle, axis);
    }
};
