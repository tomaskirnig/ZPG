#pragma once

#include <memory>
#include "Model.h"
#include "Shader.h"
#include "Transformation.h"
#include "Material.h"
#include "Texture.h"



class DrawableObject {
    private:
        std::shared_ptr<Model> model;
		std::shared_ptr<Shader> texture;
        Shader* shader;
        Material* material;
        Transformation* transformation;

        //bool isInstanced = false;         // Flag to indicate instanced rendering
        //size_t instanceCount = 0;         // Number of instances

    public:
        DrawableObject(std::shared_ptr<Model> model, std::string vertexShaderFile, std::string fragmentShaderFile);
		DrawableObject(std::shared_ptr<Model> model, std::string vertexShaderFile, std::string fragmentShaderFile, Material* material);
        ~DrawableObject();

        void draw();
        /*void drawInstanced(GLuint instanceCount);
        void setupInstancedRendering(const std::vector<glm::mat4>& transformations);*/
        
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
			transformation->scaleObject(glm::vec3 (1.0f) * scale);
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

       /* bool usesInstancedRendering() const { 
            return isInstanced; 
        }

        size_t getInstanceCount() const {
            return instanceCount; 
        }
        
        void setInstanceCount(size_t count) { 
            instanceCount = count; 
        }*/
};
