#pragma once

#include "Model.h"
#include "Shader.h"
#include "Transformation.h"

//Include objects
#include "sphere.h"
#include "tree.h"
#include "bushes.h"
#include "plain.h"
#include "suzi_flat.h"
#include "suzi_smooth.h"

class DrawableObject {
    private:
        Model* model;
        Shader* shader;

        Transformation* transformation;

    public:
        DrawableObject(const float* points, size_t sizeOfPoints, std::string vertexShaderFile, std::string fragmentShaderFile);
        DrawableObject(const float* points, size_t sizeOfPoints, std::string vertexShaderFile, std::string fragmentShaderFile, float shininess);
        ~DrawableObject();

        void draw();
        
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
};
