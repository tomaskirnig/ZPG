#include "DrawableObject.h"

DrawableObject::DrawableObject(const float* points, size_t sizeOfPoints, const char* vertexShader, const char* fragmentShader) 
    : transformation(new Transformation()) {
    model = new Model(points, sizeOfPoints);
    shader = new Shader(vertexShader, fragmentShader);
}

void DrawableObject::draw() {
    shader->use();

    glm::mat4 transformationMatrix = transformation->getMatrix();
    // Send transformation matrix to the shader program (you may need to extend your shader to accept this matrix)

    model->draw();
}

void DrawableObject::applyTransformation(Transformation* transform) {
    transformation->addChild(transform);
}
