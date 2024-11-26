#pragma once
#include <string>
#include <GL/glew.h>
#include <iostream>

#include "TextureType.h"

// Include textures
#define GRASS_TEXTURE "../Textures/grass.png"
#define WOODEN_TEXTURE "../Textures/wooden_fence.png"

using namespace std;

class Texture {
private:
    GLuint id;
	GLuint textureUnit;
    std::string name;
    std::string path;
	TextureType type;

public:
    Texture(const std::string& path, const std::string& name, int textureCount);
    Texture(const std::string& name, int textureCount);
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
