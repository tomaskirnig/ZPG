#include "Transformation.h"

Transformation::Transformation() : position(0.0f), rotationAxis(1.0f, 0.0f, 0.0f), rotationAngle(0.0f), scale(1.0f) {}

glm::mat4 Transformation::getMatrix() const {
    glm::mat4 matrix = glm::mat4(1.0f);
    matrix = glm::translate(matrix, position);
    matrix = glm::rotate(matrix, glm::radians(rotationAngle), rotationAxis);
    matrix = glm::scale(matrix, scale);

    // Apply transformations from all child nodes
    for (const auto& child : children) {
        matrix *= child->getMatrix();
    }

    return matrix;
}

void Transformation::setPosition(const glm::vec3& pos) {
    position = pos;
}

void Transformation::setRotation(float angle, const glm::vec3& axis) {
    rotationAngle = angle;
    rotationAxis = axis;
}

void Transformation::setScale(const glm::vec3& scale) {
    this->scale = scale;
}

void Transformation::addChild(Transformation* child) {
    children.push_back(child);
}

void Transformation::removeChild(Transformation* child) {
    children.erase(std::remove(children.begin(), children.end(), child), children.end());
}
