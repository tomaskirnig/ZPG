#pragma once
#include <vector>
#include "LightData.h"

class IObserver {
public:
    //virtual ~Observer() {}
    virtual void update(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const std::vector<LightData>& lights, const glm::vec3& viewPosition) = 0;  // Observer will update using the new view matrix
};
