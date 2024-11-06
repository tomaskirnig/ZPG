#pragma once

class IRotation
{
	public:
		virtual void setRotationMatrix(const glm::mat4& newRotationMatrix) = 0;
		virtual glm::mat4 getRotationMatrix() = 0;
		virtual void resetRotation() = 0;
};

