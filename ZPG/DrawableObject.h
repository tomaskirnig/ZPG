#pragma once

#include <memory>
#include "Model.h"
#include "Shader.h"
#include "Transformation.h"
#include "Material.h"
#include "Texture.h"
#include "Movement.h"

static int idCounter = 0;

const std::vector<glm::vec3> defaultBezierSegments = {
	glm::vec3(6.0f, -0.5f, -2.0f),
    glm::vec3(6.0f, 2.0f, -1.0f),
	glm::vec3(6.0f, 2.0f, -2.0f),
    glm::vec3(6.0f, -0.5f, 7.0f)
};

const std::vector<std::array<glm::vec3, 4>> defaultArcBezierSegments = {
    {{glm::vec3(0, 0, 0), glm::vec3(1, 2, 0), glm::vec3(3, 2, 0), glm::vec3(4, 0, 0)}},
    {{glm::vec3(4, 0, 0), glm::vec3(5, -2, 0), glm::vec3(7, -2, 0), glm::vec3(8, 0, 0)}},
    {{glm::vec3(8, 0, 0), glm::vec3(9, 1, 0), glm::vec3(11, 1, 0), glm::vec3(12, 0, 0)}}
};

class DrawableObject {
    private:
		int id;
        std::shared_ptr<Model> model;
		std::shared_ptr<Shader> texture;
        Shader* shader;
        Material* material;
        Transformation* transformation;
		Movement* movement;

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

        void updateMovement(float deltaTime) {
            if (movement->isMoving()) {
                glm::vec3 newPos, faceDir;
                movement->update(deltaTime, newPos, faceDir);
                if (movement->isMoving()) {
                    setPosition(newPos);

                     //Calculate yaw and pitch from faceDir
                     /*forward = -Z axis initially
                     yaw: rotation around Y-axis
                     pitch: rotation around X-axis*/

                    glm::vec3 dir = glm::normalize(faceDir);

                    if (glm::length(faceDir) < 1e-6f) {
                        // faceDir is almost zero, no rotation applied to avoid NaN
                        std::cout << "updateMovement: faceDir too small, skipping rotation.\n";
                        return;
                    }

                    // Yaw angle: angle around Y axis. atan2(x,z) used if model faces -Z initially.
                    float yaw = std::atan2(dir.x, dir.z);

                    // Pitch angle: angle around X axis, derived from vertical component
                    float pitch = std::asin(dir.y);

                    // Reset rotations before applying new ones
                    transformation->setRotation(0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
                    transformation->setRotation(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                    transformation->setRotation(0.0f, glm::vec3(0.0f, 0.0f, 1.0f));

                    transformation->rotateObject(yaw, glm::vec3(0.0f, 1.0f, 0.0f));
                    transformation->rotateObject(pitch, glm::vec3(1.0f, 0.0f, 0.0f));
                }
            }
        }

        void startLineMovement(const glm::vec3& start, const glm::vec3& end, float speed) {
            movement->setLineMovement(start, end, speed);
        }

        void startBezierMovement(const glm::vec3& p0, const glm::vec3& p1,
            const glm::vec3& p2, const glm::vec3& p3, float speed) {
            movement->setBezierMovement(p0, p1, p2, p3, speed);
        }

        void startArcBezierMovement(const std::vector<std::array<glm::vec3, 4>>& segments, float speed) {
            movement->setArcBezierMovement(segments, speed);
        }
};
