#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>  // for glm::value_ptr
#include <vector>

using namespace std;

class Transformation
{
    private:
        glm::vec3 position;
       /* glm::vec3 rotationAxis;
        float rotationAngle;*/
        glm::vec3 scale;
        glm::mat4 rotationMatrix;
        glm::mat4 matrix;

        vector<Transformation*> children;

        void updateMatrix();

    public:
        Transformation();

        glm::mat4 getMatrix() const;

        void setPosition(const glm::vec3& pos);
		glm::vec3 getPosition();

        void setScale(const glm::vec3& scale);
        glm::vec3 getScale();

		void setRotationMatrix(const glm::mat4& newRotationMatrix);
        void applyRotationMatrix(const glm::mat4& newRotationMatrix);
		glm::mat4 getRotationMatrix();
        void resetRotation();

        // Composite methods
        void addChild(Transformation* child);
        void removeChild(Transformation* child);
};

