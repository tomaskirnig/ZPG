#pragma once

#include "Model.h"
#include "Shader.h"
#include "Transformation.h"

class DrawableObject {
    private:
        Model* model;
        Shader* shader;

        Transformation* transformation;

    public:
        DrawableObject(const float* points, size_t size, const char* vertexShader, const char* fragmentShader);

        void draw();

        void applyTransformation(Transformation* transform);

        Transformation* getTransformation();

		// Get the shader of the object
        Shader* getShader() {
            return this->shader;
        }
};
