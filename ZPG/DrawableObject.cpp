#include "DrawableObject.h"

DrawableObject::DrawableObject(const float* points, size_t sizeOfPoints, string vertexShaderFile, string fragmentShaderFile)
    : transformation(new Transformation()) {
    model = new Model(points, sizeOfPoints);
    shader = new Shader(vertexShaderFile, fragmentShaderFile);
}

// Draw the object
void DrawableObject::draw() {
    shader->use(); // Set the shader to be used

    glm::mat4 transformationMatrix = transformation->getMatrix();

    // Get the uniform location
    GLint transformLoc = shader->getUniformLocation("transformationMatrix"); 

    // Send the transformation matrix to the shader
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformationMatrix));
    model->draw();
}

// Apply a transformation to the object
void DrawableObject::applyTransformation(Transformation* transform) {
    // Update the existing transformation
    transformation->setPosition(transform->getPosition());
    transformation->setRotationMatrix(transform->getRotationMatrix());
    transformation->setScale(transform->getScale());
}

// Get the transformation of the object
Transformation* DrawableObject::getTransformation() {
    return transformation;
}
