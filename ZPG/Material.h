#pragma once
#include <glm/glm.hpp>
#include <memory>
#include "Texture.h"

class Material {
private:
    glm::vec3 ambient;   // ra
    glm::vec3 diffuse;   // rd
    glm::vec3 specular;  // rs
    float shininess;
	std::shared_ptr<Texture> texture;

public:
	Material(const glm::vec3& ambient = glm::vec3(0.1f),
        const glm::vec3& diffuse = glm::vec3(1.0f),
        const glm::vec3& specular = glm::vec3(1.0f),
        float shininess = 32.0f,
		std::shared_ptr<Texture> texture = nullptr)
        : ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {
		if (texture) {
			this->texture = texture;
		}
	}

	void setAmbient(const glm::vec3& ambient) {
		this->ambient = ambient;
	}

	void setDiffuse(const glm::vec3& diffuse) {
		this->diffuse = diffuse;
	}

	void setSpecular(const glm::vec3& specular) {
		this->specular = specular;
	}

	void setShininess(float shininess) {
		this->shininess = shininess;
	}

	glm::vec3 getAmbient() const {
		return ambient;
	}

	glm::vec3 getDiffuse() const {
		return diffuse;
	}

	glm::vec3 getSpecular() const {
		return specular;
	}

	float getShininess() const {
		return shininess;
	}

	std::shared_ptr<Texture> getTexture() const {
		return texture;
	}

	void setTexture(std::shared_ptr<Texture> texture) {
		this->texture = texture;
	}

	GLuint getTextureID() const {
		if (texture) {
			return texture->getID();
		}
		return 0;
	}

	GLuint getTextureUnit() const {
		if (texture) {
			return texture->getTextureUnit();
		}
		return 0;
	}

	TextureType getTextureType() const {
		if (texture) {
			return texture->getType();
		}
		return TextureType::Texture2D;
	}

	void bindTexture(GLuint unit) const {
		if (texture) {
			glActiveTexture(GL_TEXTURE0 + unit);
			texture->bind();
		}
	}
};
