#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include "Texture.h"

class TextureManager {
private:
    std::unordered_map<std::string, std::shared_ptr<Texture>> textures;

public:
    std::shared_ptr<Texture> getTexture(const std::string& texturePath, const std::string& textureName);
};
