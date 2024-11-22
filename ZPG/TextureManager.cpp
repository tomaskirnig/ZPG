#include "TextureManager.h"

std::shared_ptr<Texture> TextureManager::getTexture(const std::string& texturePath, const std::string& textureName) {
    auto it = textures.find(texturePath);
    if (it != textures.end()) {
        return it->second;
    }
    else {
        auto texture = std::make_shared<Texture>(texturePath, textureName);
        textures[texturePath] = texture;
        cout << "Texture loaded: " << textureName << endl;
        return texture;
    }
}
