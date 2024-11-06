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
        DrawableObject(const float* points, size_t sizeOfPoints, string vertexShaderFile, string fragmentShaderFile);
        DrawableObject(const float* points, size_t sizeOfPoints, string vertexShaderFile, string fragmentShaderFile, float shininess);

        void draw();
        
        void applyTransformation(Transformation* transform);

        Transformation* getTransformation();

		void moveObject(const glm::vec3& direction) {
			transformation->setPosition(transformation->getPosition() + direction);
		}

		void setPosition(const glm::vec3& pos) {
			transformation->setPosition(pos);
		}

        void rotateObject(const glm::vec3& axis, float angle) {
			transformation->setRotationMatrix(glm::rotate(transformation->getRotationMatrix(), glm::radians(angle), axis));
		}

        void scaleObject(const float scale) {
			transformation->setScale(transformation->getScale() + scale);
		}

		glm::vec3 getPosition() {
			return transformation->getPosition();
		}

		// Get the shader of the object
        Shader* getShader() {
            return this->shader;
        }
};
