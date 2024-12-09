#include "Movement.h"
#include <algorithm>

// Utility functions for interpolation
static glm::vec3 lerp(const glm::vec3& a, const glm::vec3& b, float t) {
    return a + t * (b - a);
}

static glm::vec3 cubicBezier(const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& P2, const glm::vec3& P3, float t) {
    float u = 1.0f - t;

    glm::vec3 point = u * u * u * P0;
    point += 3 * u * u * t * P1;
    point += 3 * u * t * t * P2;
    point += t * t * t * P3;

    return point;
}

void Movement::setLineMovement(const glm::vec3& start, const glm::vec3& end, float speed) {
    type = MovementType::LINEAR;
    lineStart = start;
    lineEnd = end;
    lineSpeed = speed;
    lineT = 0.0f;
    moving = true;
}

void Movement::setBezierMovement(const glm::vec3& p0, const glm::vec3& p1,
    const glm::vec3& p2, const glm::vec3& p3, float speed) {
    type = MovementType::BEZIER;
    P0 = p0; P1 = p1; P2 = p2; P3 = p3;
    bezierSpeed = speed;
    bezierT = 0.0f;
    moving = true;
}

void Movement::setArcBezierMovement(const std::vector<std::array<glm::vec3, 4>>& segments, float speed) {
    type = MovementType::ARC_BEZIER;
    arcSegments = segments;
    arcSpeed = speed;
    arcT = 0.0f;
    currentSegmentIndex = 0;
    moving = true;
	direction = 1.0f;
}

void Movement::reverse(MovementType type)
{
	if (type == MovementType::LINEAR) {
		std::swap(lineStart, lineEnd);
		lineT = 1.0f - lineT;
	}
	else if (type == MovementType::BEZIER) {
		std::swap(P0, P3);
		std::swap(P1, P2);
		bezierT = 1.0f - bezierT;
	}
    else if (type == MovementType::ARC_BEZIER) {
		direction *= -1.0f;
    }
}

glm::vec3 Movement::update(float deltaTime, glm::vec3& newPos, glm::vec3& faceDir) {
    if (!moving || type == MovementType::NONE) return glm::vec3(0.0f);

    switch (type) {
    case MovementType::LINEAR: {
        lineT += deltaTime * lineSpeed;
        float t = std::max(0.0f, std::min(lineT, 1.0f));
        glm::vec3 pos = lerp(lineStart, lineEnd, t);
        if (t >= 1.0f) reverse(MovementType::LINEAR); 
        //stop();
        newPos = pos;
		faceDir = linearDerivative(lineStart, lineEnd);
		break;
    }
    case MovementType::BEZIER: {
        bezierT += deltaTime * bezierSpeed;
        float t = std::max(0.0f, std::min(bezierT, 1.0f));
        glm::vec3 pos = cubicBezier(P0, P1, P2, P3, t);
        if (t >= 1.0f) reverse(MovementType::BEZIER); 
        //stop();
        newPos = pos;
		faceDir = cubicBezierDerivative(P0, P1, P2, P3, t);
		break;
    }
    case MovementType::ARC_BEZIER: {
        if (arcSegments.empty()) {
            //reverse(MovementType::ARC_BEZIER);
            return glm::vec3(0.0f);
        }

        arcT += deltaTime * arcSpeed * direction;
        float t = std::max(0.0f, std::min(arcT, 1.0f));

        const auto& seg = arcSegments[currentSegmentIndex];
        glm::vec3 pos = cubicBezier(seg[0], seg[1], seg[2], seg[3], t);

        if (t >= 1.0f || t <= 0.0f) {
            if (direction > 0.0f) {
                // Move to the next segment
                currentSegmentIndex++;
                if (currentSegmentIndex >= (int)arcSegments.size()) {
                    reverse(MovementType::ARC_BEZIER);
                    currentSegmentIndex = arcSegments.size() - 1; // Go to the last segment
                }
            }
            else {
                // Move to the previous segment
                currentSegmentIndex--;
                if (currentSegmentIndex < 0) {
                    reverse(MovementType::ARC_BEZIER);
                    currentSegmentIndex = 0; // Go to the first segment
                }
            }

            // Reset `arcT` for the new segment
            arcT = (direction > 0.0f) ? 0.0f : 1.0f;
        }

        newPos = pos;
		faceDir = cubicBezierDerivative(seg[0], seg[1], seg[2], seg[3], t);
		break;
    }
    default:
        newPos = glm::vec3(0.0f);
		faceDir = glm::vec3(0.0f);
		break;
    }
}

glm::vec3 Movement::cubicBezierDerivative(const glm::vec3& P0, const glm::vec3& P1,
    const glm::vec3& P2, const glm::vec3& P3, float t) {
    float u = 1.0f - t;

    glm::vec3 tangent = 3.0f * u * u * (P1 - P0) + 
        6.0f * u * t * (P2 - P1) + 
        3.0f * t * t * (P3 - P2);

    return tangent;
}