#pragma once
#include <string>
#include <GL/glew.h>
#include <iostream>
#include <SOIL.h>

#include "TextureType.h"

#define TEXTURE_PATH "../Textures/"
#define TEXTURE_PATH_CUBE "../Textures/cubemap/"

using namespace std;

class Texture {
private:
    GLuint id;
	GLuint textureUnit;
    std::string name;
	TextureType type;

public:
    Texture(const std::string& name, int textureCount, int ver);
    ~Texture();

	GLuint getID() const {
		return id;
	}

	GLuint getTextureUnit() const {
		return textureUnit;
	}

	TextureType getType() const {
		return type;
	}

    void bind();
};
