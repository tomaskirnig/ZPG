#pragma once

#include "Model.h"
#include "Shader.h"
#include "Transformation.h"

class DrawableObject {
    private:
        Model* model;
        Shader* shader;

        // Root Transformation object (Composite pattern)
        Transformation* transformation;

    public:
        DrawableObject(const float* points, size_t size, const char* vertexShader, const char* fragmentShader);

        void draw();

        void applyTransformation(Transformation* transform);

        Transformation* getTransformation();
};