#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <vector>

#include "BaseTransformation.h"
#include "Translation.h"
#include "Rotation.h"
#include "Scale.h"

using namespace std;

class Transformation : public BaseTransformation
{
    private:
        // (Composite pattern)
        std::vector<BaseTransformation*> transformations;

    public:
        Transformation();
        ~Transformation();

        glm::mat4 getMatrix() const override;

        void addTransformation(BaseTransformation* child) {
            transformations.push_back(child);
        }

        void removeTransformation(BaseTransformation* child) {
            transformations.erase(std::remove(transformations.begin(), transformations.end(), child), transformations.end());
        }

		// Position
        void moveObject(const glm::vec3& offset);
        void setPosition(const glm::vec3& position);
		glm::vec3 getPosition();

		// Scale
		void scaleObject(const glm::vec3& scale);
        void setScale(const glm::vec3& scale);

		// Rotation
		void rotateObject(float angleDegrees, const glm::vec3& axis);
		void setRotation(float angleDegrees, const glm::vec3& axis);
};

