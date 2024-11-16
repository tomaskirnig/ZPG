#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include "Model.h"

class ModelManager {
private:
    std::unordered_map<std::string, std::shared_ptr<Model>> models;

public:
    std::shared_ptr<Model> getModel(const std::string& modelName, const float* points, size_t size);
    void clearModels() { models.clear(); };
};