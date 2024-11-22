#pragma once
#include <string>
#include <GL/glew.h>
#include <iostream>

// Include textures
#define GRASS_TEXTURE "../Textures/grass.png"
#define WOODEN_TEXTURE "../Textures/wooden_fence.png"

using namespace std;

class Texture {
private:
    GLuint id;
    std::string type;
    std::string path;

public:
    Texture(const std::string& path, const std::string& typeName);
    ~Texture();

	GLuint getID() const {
		return id;
	}

    void bind(GLuint unit = 0) const;
};
