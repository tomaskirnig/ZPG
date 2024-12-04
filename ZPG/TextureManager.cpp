#include "TextureManager.h"

std::shared_ptr<Texture> TextureManager::getTexture(const std::string& textureName) {
    auto it = textures.find(textureName);
    if (it != textures.end()) {
        return it->second;
    }
    else {
        auto texture = std::make_shared<Texture>(textureName, textures.size(), 0);
        textures[textureName] = texture;
        cout << "Texture loaded: " << textureName << endl;
        return texture;
    }
}

std::shared_ptr<Texture> TextureManager::getCubeMap() {
	auto it = textures.find("skybox");
	if (it != textures.end()) {
		return it->second;
	}
	else {
		auto texture = std::make_shared<Texture>("skybox", textures.size(), 1);
		textures["skybox"] = texture;
		cout << "Skybox loaded" << endl;
		return texture;
	}
}
