#pragma once

#include <glm/glm.hpp>

class Observer {
public:
    //virtual ~Observer() {}
    virtual void update(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) = 0;  // Observer will update using the new view matrix
};
