#include "DrawableObject.h"

DrawableObject::DrawableObject(const float* points, size_t sizeOfPoints, std::string vertexShaderFile, std::string fragmentShaderFile)
    : transformation(new Transformation()) {
    model = new Model(points, sizeOfPoints);
    shader = new Shader(vertexShaderFile, fragmentShaderFile);
    //transformation = new Transformation();
}

DrawableObject::DrawableObject(const float* points, size_t sizeOfPoints, std::string vertexShaderFile, std::string fragmentShaderFile, float shininess)
    : transformation(new Transformation()) {
    model = new Model(points, sizeOfPoints);
    shader = new Shader(vertexShaderFile, fragmentShaderFile, shininess);
}

DrawableObject::~DrawableObject() {
    delete model;
    delete shader;
    delete transformation;
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

// Get the transformation of the object
glm::mat4 DrawableObject::getTransformationMatrix() {
    return transformation->getMatrix();
}
