#include "DrawableObject.h"

DrawableObject::DrawableObject(std::shared_ptr<Model> model, std::string vertexShaderFile, std::string fragmentShaderFile)
    : model(model), transformation(new Transformation()), material(new Material()) {
    shader = new Shader(vertexShaderFile, fragmentShaderFile);
	texture = nullptr;
}

DrawableObject::DrawableObject(std::shared_ptr<Model> model, std::string vertexShaderFile, std::string fragmentShaderFile, Material* material)
	: model(model), transformation(new Transformation()){
	shader = new Shader(vertexShaderFile, fragmentShaderFile, material->getShininess());
	
	if (material != nullptr) this->material = material;
	else this->material = new Material();
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

void DrawableObject::draw(bool following, glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {
    if (model != nullptr) {
        shader->use(); // Set the shader to be used
        if (following) {
            viewMatrix = glm::mat4(glm::mat3(viewMatrix));
        }

        // Get the uniform location
        GLint viewLoc = shader->getUniformLocation("viewMatrix");
        GLint projectionLoc = shader->getUniformLocation("projectionMatrix");

		if (viewLoc == -1 || projectionLoc == -1) cout << "Error: view or projection matrix not found" << endl;
        
        // Set material uniforms
        shader->setMaterial(material);

        // Send the matrixes to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        model->draw();
	}
	else cout << "Error: Model not found" << endl;
}

// Get the transformation of the object
glm::mat4 DrawableObject::getTransformationMatrix() {
    return transformation->getMatrix();
}
