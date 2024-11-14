#pragma once

#include "BaseTransformation.h"
#include <glm/gtc/matrix_transform.hpp>

class Translation : public BaseTransformation {
public:
    glm::vec3 offset;

    Translation(const glm::vec3& offset) : offset(offset) {}

    glm::mat4 getMatrix() const override {
        return glm::translate(glm::mat4(1.0f), offset);
    }
};
