#include "DrawableObject.h"

DrawableObject::DrawableObject(const float* points, size_t sizeOfPoints, const char* vertexShader, const char* fragmentShader) 
    : transformation(new Transformation()) {
    model = new Model(points, sizeOfPoints);
    shader = new Shader(vertexShader, fragmentShader);
}

void DrawableObject::draw() {
    shader->use();

    glm::mat4 transformationMatrix = transformation->getMatrix();

    // Get the cached uniform location (or retrieve it if it hasn't been cached yet)
    GLint transformLoc = shader->getUniformLocation("transformationMatrix");

    // Send the transformation matrix to the shader
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformationMatrix));
    model->draw();
}

void DrawableObject::applyTransformation(Transformation* transform) {
    // Update the existing transformation
    transformation->setPosition(transform->getPosition());
    transformation->setRotationMatrix(transform->getRotationMatrix());
    transformation->setScale(transform->getScale());
}

Transformation* DrawableObject::getTransformation() {
    return transformation;
}
