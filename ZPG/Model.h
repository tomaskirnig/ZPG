#pragma once
#include <GL/glew.h>
#include <cstddef>
#include <iostream>


class Model {
public:
    GLuint VAO, VBO;
    size_t numVertices;

    Model(const float* points, size_t size);
    void draw();
    void deleteModel();
};
