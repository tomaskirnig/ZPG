#include "DrawableObject.h"

DrawableObject::DrawableObject(std::shared_ptr<Model> model, std::string vertexShaderFile, std::string fragmentShaderFile)
    : model(model), transformation(new Transformation()) {
    shader = new Shader(vertexShaderFile, fragmentShaderFile);
	material = new Material();
}

DrawableObject::DrawableObject(std::shared_ptr<Model> model, std::string vertexShaderFile, std::string fragmentShaderFile, Material* material)
	: model(model), transformation(new Transformation()){
	shader = new Shader(vertexShaderFile, fragmentShaderFile, material->getShininess());
	
    if (!material) this->material = new Material();
	else this->material = material;
}

DrawableObject::~DrawableObject() {
    delete shader;
    delete transformation;
	delete material;
}

// Draw the object
void DrawableObject::draw() {
    if (model != nullptr) {
        shader->use(); // Set the shader to be used

        glm::mat4 transformationMatrix = transformation->getMatrix();

        // Get the uniform location
        GLint transformLoc = shader->getUniformLocation("transformationMatrix");

        // Set material uniforms
        shader->setMaterial(material);

        // Send the transformation matrix to the shader
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformationMatrix));
        model->draw();
    }
}

void DrawableObject::drawInstanced(GLuint instanceCount) {
    shader->use();
    model->drawInstanced(instanceCount);
}

void DrawableObject::setupInstancedRendering(const std::vector<glm::mat4>& transformations) {
    if (transformations.empty()) return; // No transformations to set up

    isInstanced = true;
    instanceCount = transformations.size();
    model->setupInstanceBuffer(transformations);
}

// Get the transformation of the object
glm::mat4 DrawableObject::getTransformationMatrix() {
    return transformation->getMatrix();
}
