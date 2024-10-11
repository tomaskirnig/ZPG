#pragma once
#include <GL/glew.h>
#include <cstddef>
#include <iostream>

using namespace std;

class Model {
    private:
        GLuint VAO, VBO;
        size_t objectSize;

    public:
        Model(const float* points, size_t size);
        void draw();
        void deleteModel();
};
