#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <array>
#include <iostream> 
#include <algorithm> 
#include "MovementType.h"


// Everything in one class so the object can have every movement type at the same time

class Movement {
private:
    MovementType type;

    // Common
    bool moving = false;

    // Linear
    glm::vec3 lineStart, lineEnd;
    float lineSpeed;
    float lineT;

    // Bezier
    glm::vec3 P0, P1, P2, P3;
    float bezierSpeed;
    float bezierT;

    // Arc-Bezier
    std::vector<std::array<glm::vec3, 4>> arcSegments;
    int currentSegmentIndex;
    float arcSpeed;
    float arcT;
    float direction;

public:
    Movement() : type(MovementType::NONE), moving(false) {}

	void setMovementType(MovementType type) { this->type = type; }

    void setLineMovement(const glm::vec3& start, const glm::vec3& end, float speed);
    void setBezierMovement(const glm::vec3& p0, const glm::vec3& p1,const glm::vec3& p2, const glm::vec3& p3, float speed);
    void setArcBezierMovement(const std::vector<std::array<glm::vec3, 4>>& segments, float speed);

    void start() { moving = true; }
    void stop() { moving = false; }
    void reverse(MovementType type);
    bool isMoving() const { return moving; }

    void update(float deltaTime, glm::vec3& newPos, glm::vec3& faceDir);
    MovementType getType() const { return type; }

    glm::vec3 linearDerivative(const glm::vec3& start, const glm::vec3& end) {
        return glm::normalize(end - start);
    }

    glm::vec3 cubicBezierDerivative(const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& P2, const glm::vec3& P3, float t);
};
