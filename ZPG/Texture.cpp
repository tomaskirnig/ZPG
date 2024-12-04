#include "Texture.h"
#include <iostream>

Texture::Texture(const std::string& name, int textureCount, int ver) 
    : id(0), name(name), textureUnit(textureCount), type(TextureType::Texture2D) {
    if (ver == 0) {
        id = SOIL_load_OGL_texture(
            (TEXTURE_PATH + name).c_str(),
            SOIL_LOAD_RGBA,    
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_INVERT_Y 
        );

        if (id == 0) {
            std::cerr << "Failed to load texture at path: " << name << "\nSOIL error: " << SOIL_last_result() << std::endl;
        }
        else {
            // Set texture parameters (optional)
            glBindTexture(GL_TEXTURE_2D, id);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
    else {
        id = SOIL_load_OGL_cubemap(
            (TEXTURE_PATH_CUBE + (std::string)"posx.jpg").c_str(),
            (TEXTURE_PATH_CUBE + (std::string)"negx.jpg").c_str(),
            (TEXTURE_PATH_CUBE + (std::string)"posy.jpg").c_str(),
            (TEXTURE_PATH_CUBE + (std::string)"negy.jpg").c_str(),
            (TEXTURE_PATH_CUBE + (std::string)"posz.jpg").c_str(),
            (TEXTURE_PATH_CUBE + (std::string)"negz.jpg").c_str(),
            SOIL_LOAD_RGB,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS
        );

        if (id == 0) {
            std::cerr << "Failed to load cube map at pathCube: " << TEXTURE_PATH_CUBE << "\nSOIL error: " << SOIL_last_result() << std::endl;
        }
        else {
			type = TextureType::CubeMap;
            // Set texture parameters (optional)
            glBindTexture(GL_TEXTURE_CUBE_MAP, id); //GL_TEXTURE_2D

            //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  // Add this
            //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);  //GL_TEXTURE_2D 

            //glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        }
    }
    
}

Texture::~Texture() {
    glDeleteTextures(1, &id);
}

void Texture::bind() {
    if (type == TextureType::CubeMap) {
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    }
    else {
        glBindTexture(GL_TEXTURE_2D, id);
    }
}
