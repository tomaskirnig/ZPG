#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>  // for glm::value_ptr
#include <vector>

#include "ITranslation.h"
#include "IScale.h"
#include "IRotation.h"

using namespace std;

class Transformation : ITranslation, IScale, IRotation
{
    private:
        glm::vec3 position;
        glm::vec3 scale;
        glm::mat4 rotationMatrix;
        glm::mat4 matrix;

        // (Composite pattern)
        vector<Transformation*> children;

        void updateMatrix();

    public:
        Transformation();

        glm::mat4 getMatrix() const;

		// Position
        void setPosition(const glm::vec3& pos);
		glm::vec3 getPosition();

		// Scale
        void setScale(const glm::vec3& scale);
        glm::vec3 getScale();
		void resetScale();

		// Rotation
		void setRotationMatrix(const glm::mat4& newRotationMatrix);
		glm::mat4 getRotationMatrix();
        void resetRotation();

        // Composite methods
        void addChild(Transformation* child);
        void removeChild(Transformation* child);
};

