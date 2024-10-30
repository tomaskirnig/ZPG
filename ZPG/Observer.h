#pragma once
#include <glm/glm.hpp>

class Observer {
public:
    //virtual ~Observer() {}
    virtual void update(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, glm::vec3 lightPosition, glm::vec3 lightColor) = 0;  // Observer will update using the new view matrix
};
