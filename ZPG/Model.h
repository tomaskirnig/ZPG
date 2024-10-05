#pragma once
#include <GL/glew.h>
#include <cstddef>

class Model {
public:
    GLuint VAO, VBO;
    size_t numVertices;

    Model(float* points, size_t size);
    void draw();
    void deleteModel();
};
