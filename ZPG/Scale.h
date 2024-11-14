#pragma once

#include "BaseTransformation.h"
#include <glm/gtc/matrix_transform.hpp>

class Scale : public BaseTransformation {
public:
    glm::vec3 scaleFactor;

    Scale(const glm::vec3& scaleFactor) : scaleFactor(scaleFactor) {}

    glm::mat4 getMatrix() const override {
        return glm::scale(glm::mat4(1.0f), scaleFactor);
    }
};
