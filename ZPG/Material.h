// Material.h
#pragma once
#include <glm/glm.hpp>

class Material {
private:
    glm::vec3 ambient;   // ra
    glm::vec3 diffuse;   // rd
    glm::vec3 specular;  // rs
    float shininess;

public:
	Material(const glm::vec3& ambient = glm::vec3(0.1f),
        const glm::vec3& diffuse = glm::vec3(1.0f),
        const glm::vec3& specular = glm::vec3(1.0f),
        float shininess = 32.0f)
        : ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {}

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
};
