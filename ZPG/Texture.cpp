#include "Texture.h"
#include <SOIL.h>
#include <iostream>

Texture::Texture(const std::string& path, const std::string& typeName)
    : id(0), type(typeName), path(path) {
    // Load texture 
    id = SOIL_load_OGL_texture(
        path.c_str(),
		SOIL_LOAD_AUTO,     //SOIL_LOAD_RGBA 
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS  //SOIL_FLAG_INVERT_Y
    );

    if (id == 0) {
        std::cerr << "Failed to load texture at path: " << path << "\nSOIL error: " << SOIL_last_result() << std::endl;
    }
    else {
        // Set texture parameters (optional)
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Adjust 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Adjust 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Adjust 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Adjust

        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

Texture::~Texture() {
    glDeleteTextures(1, &id);
}

void Texture::bind(GLuint unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id);
}
