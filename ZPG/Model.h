#pragma once
#include <GL/glew.h>
#include <cstddef>

class Model {
public:
    GLuint VAO, VBO; // = 0;
    size_t numVertices;

    Model(float* points, size_t size);
    void draw();
    void deleteModel();
};
