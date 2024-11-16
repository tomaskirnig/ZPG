#pragma once
#include <glm/glm.hpp>
#include "LightType.h"

struct LightData {
    glm::vec3 position;
    glm::vec3 direction; // For directional and spotlights
    glm::vec3 color;
    float intensity;
    float cutOff;       // For spotlights
    float outerCutOff;  // For spotlights
    LightType type;
};
