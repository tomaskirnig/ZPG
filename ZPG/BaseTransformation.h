#pragma once

#include <glm/glm.hpp>

class BaseTransformation {
public:
    virtual ~BaseTransformation() = default;

    virtual glm::mat4 getMatrix() const = 0;
};
