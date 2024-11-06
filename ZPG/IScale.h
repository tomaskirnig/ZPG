#pragma once

class IScale
{
    public:
        virtual void setScale(const glm::vec3& scale) = 0;
        virtual glm::vec3 getScale() = 0;
        virtual void resetScale() = 0;
};


