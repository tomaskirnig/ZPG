#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

using namespace std;

class Transformation
{
    private:
        glm::vec3 position;
        glm::vec3 rotationAxis;
        float rotationAngle;
        glm::vec3 scale;

        vector<Transformation*> children;

    public:
        Transformation();
        glm::mat4 getMatrix() const;

        void setPosition(const glm::vec3& pos);
        void setRotation(float angle, const glm::vec3& axis);
        void setScale(const glm::vec3& scale);

        // Composite methods
        void addChild(Transformation* child);
        void removeChild(Transformation* child);
};

