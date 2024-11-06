#include "Transformation.h"


Transformation::Transformation() : position(0.0f), rotationMatrix(1.0f), scale(glm::vec3(0.5f)) {
	updateMatrix();
}

// Returns the transformation matrix
glm::mat4 Transformation::getMatrix() const {
    return matrix;
}

// Sets new position
void Transformation::setPosition(const glm::vec3& pos) {
    position = pos;
    updateMatrix();
}

// Returns the position
glm::vec3 Transformation::getPosition(){
    return position;
}

// Sets new rotation matrix
void Transformation::setRotationMatrix(const glm::mat4& newRotationMatrix) {
	rotationMatrix = newRotationMatrix;
	updateMatrix();
}
// Returns the rotation matrix
glm::mat4 Transformation::getRotationMatrix() {
    return rotationMatrix;
}

// Sets new scale
void Transformation::setScale(const glm::vec3& scale) {
    this->scale = scale;
    updateMatrix();
}

// Returns the scale
glm::vec3 Transformation::getScale(){
    return scale;
}

// Updates the transformation matrix
void Transformation::updateMatrix() {
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
    matrix = translationMatrix * rotationMatrix * scaleMatrix;
}

// Resets the rotation matrix
void Transformation::resetRotation() {
    rotationMatrix = glm::mat4(1.0f); // Reset rotation to identity matrix
    updateMatrix();
}

// Resets the scale
void Transformation::resetScale() {
	scale = glm::vec3(0.5f); // Reset scale to 0.5
	updateMatrix();
}

// Adds a child to the transformation
void Transformation::addChild(Transformation* child) {
    children.push_back(child);
}

// Removes a child from the transformation
void Transformation::removeChild(Transformation* child) {
    children.erase(std::remove(children.begin(), children.end(), child), children.end());
}
