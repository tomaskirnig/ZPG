#include "DrawableObject.h"

DrawableObject::DrawableObject(std::shared_ptr<Model> model, std::string vertexShaderFile, std::string fragmentShaderFile, bool genId)
    : model(model), transformation(new Transformation()), material(new Material()), movement(new Movement()) {
	if (genId) id = generateId();
	else id = 0;
    shader = new Shader(vertexShaderFile, fragmentShaderFile);
	texture = nullptr;

}

DrawableObject::DrawableObject(std::shared_ptr<Model> model, std::string vertexShaderFile, std::string fragmentShaderFile, Material* material, bool genId)
	: model(model), transformation(new Transformation()), movement(new Movement()) {

	if (genId) id = generateId();
	else id = 0;

	shader = new Shader(vertexShaderFile, fragmentShaderFile);
	
	if (material != nullptr) this->material = material;
	else this->material = new Material();
}

DrawableObject::~DrawableObject() {
    delete shader;
    delete transformation;
	delete material;
    delete movement;
}

// Draw the object
void DrawableObject::draw() {
    if (model != nullptr) {
        shader->use(); 

        glStencilFunc(GL_ALWAYS, this->getId(), 0xFF);

        glm::mat4 transformationMatrix = transformation->getMatrix();

        // Get the uniform location
        GLint transformLoc = shader->getUniformLocation("transformationMatrix");

        shader->setMaterial(material);

        // Send the transformation matrix to the shader
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformationMatrix));
        model->draw();
    }
}

void DrawableObject::draw(bool following, glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {
    if (model != nullptr) {
        shader->use();
        if (following) {
			viewMatrix = glm::mat4(glm::mat3(viewMatrix));// Throw out the transaltion part
        }

        GLint viewLoc = shader->getUniformLocation("viewMatrix");
        GLint projectionLoc = shader->getUniformLocation("projectionMatrix");

		if (viewLoc == -1 || projectionLoc == -1) cout << "Error: view or projection matrix not found" << endl;
        
        shader->setMaterial(material);

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

