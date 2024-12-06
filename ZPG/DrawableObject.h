#pragma once

#include <memory>
#include "Model.h"
#include "Shader.h"
#include "Transformation.h"
#include "Material.h"
#include "Texture.h"

static int idCounter = 0;

class DrawableObject {
    private:
		int id;
        std::shared_ptr<Model> model;
		std::shared_ptr<Shader> texture;
        Shader* shader;
        Material* material;
        Transformation* transformation;

		int generateId() {
			return ++idCounter;
		}

    public:
        DrawableObject(std::shared_ptr<Model> model, std::string vertexShaderFile, std::string fragmentShaderFile, bool genId);
		DrawableObject(std::shared_ptr<Model> model, std::string vertexShaderFile, std::string fragmentShaderFile, Material* material, bool genId);
        ~DrawableObject();

        void draw();
		void draw(bool following, glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

		int getId() { return id; }

        glm::mat4 getTransformationMatrix();

		void moveObject(const glm::vec3& direction) {
			transformation->moveObject(direction);
		}

		void setPosition(const glm::vec3& pos) {
			transformation->setPosition(pos);
		}

        void rotateObject(const glm::vec3& axis, float angle) {
			transformation->rotateObject(glm::radians(angle), axis);
		}

        void setRotation(const glm::vec3& axis, float angle) {
			transformation->setRotation(glm::radians(angle), axis);
        }

        void scaleObject(const float scale) {
			transformation->scaleObject(glm::vec3 (scale));
		}

        void setScale(const float scale) {
            transformation->setScale(glm::vec3(1.0f) * scale);
        }

		glm::vec3 getPosition() {
			return transformation->getPosition();
		}

		// Get the shader of the object
        Shader* getShader() {
            return this->shader;
        }

		std::shared_ptr<Model> getModel() {
			return this->model;
		}

		std::shared_ptr<Shader> getTexture() {
			return this->texture;
		}
};
