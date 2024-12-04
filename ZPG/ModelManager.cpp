#include "ModelManager.h"

std::shared_ptr<Model> ModelManager::getModel(const std::string& modelName, const float* points, size_t size, int ver) {
    // Check if the model is already loaded
    auto it = models.find(modelName);
    if (it != models.end()) {
        return it->second;
    }

    // If not, load the model and store it
    std::shared_ptr<Model> model = std::make_shared<Model>(points, size, ver);
    models[modelName] = model;
    cout << "Model loaded: " << modelName << endl;
    return model;
}

std::shared_ptr<Model> ModelManager::getModel(const std::string& fileName) {
    auto it = models.find(fileName);
    if (it != models.end()) {
        return it->second;
    }

    std::shared_ptr<Model> model = std::make_shared<Model>(fileName);
    models[fileName] = model;
    cout << "Model loaded: " << fileName << endl;
    return model;
}