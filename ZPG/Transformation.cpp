#include "Transformation.h"


Transformation::Transformation() : position(0.0f), rotationMatrix(1.0f), scale(glm::vec3(0.5f)) {
	updateMatrix();
}

glm::mat4 Transformation::getMatrix() const {
    return matrix;
}

void Transformation::setPosition(const glm::vec3& pos) {
    position = pos;
    updateMatrix();
}

glm::vec3 Transformation::getPosition(){
    return position;
}

void Transformation::setRotationMatrix(const glm::mat4& newRotationMatrix) {
	rotationMatrix = newRotationMatrix;
	updateMatrix();
}

void Transformation::applyRotationMatrix(const glm::mat4& newRotationMatrix) {
    rotationMatrix = rotationMatrix * newRotationMatrix;  // Multiply the new rotation by the existing one
    updateMatrix();  // Recalculate the final transformation matrix
}

glm::mat4 Transformation::getRotationMatrix() {
    return rotationMatrix;
}

void Transformation::setScale(const glm::vec3& scale) {
    this->scale = scale;
    updateMatrix();
}

glm::vec3 Transformation::getScale(){
    return scale;
}

void Transformation::updateMatrix() {
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
    matrix = translationMatrix * rotationMatrix * scaleMatrix;
}

void Transformation::resetRotation() {
    rotationMatrix = glm::mat4(1.0f); // Reset rotation to identity matrix
    updateMatrix();
}

void Transformation::addChild(Transformation* child) {
    children.push_back(child);
}

void Transformation::removeChild(Transformation* child) {
    children.erase(std::remove(children.begin(), children.end(), child), children.end());
}
