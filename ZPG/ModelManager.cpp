#include "ModelManager.h"

std::shared_ptr<Model> ModelManager::getModel(const std::string& modelName, const float* points, size_t size) {
    // Check if the model is already loaded
    auto it = models.find(modelName);
    if (it != models.end()) {
        return it->second;
    }

    // If not, load the model and store it
    std::shared_ptr<Model> model = std::make_shared<Model>(points, size);
    models[modelName] = model;
    return model;
}
