#pragma once

class ITranslation
{
	public:
		virtual void setPosition(const glm::vec3& pos) = 0;
		virtual glm::vec3 getPosition() = 0;
};

