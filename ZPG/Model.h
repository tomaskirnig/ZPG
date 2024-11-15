#pragma once
#include <GL/glew.h>
#include <cstddef>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

using namespace std;

class Model {
    private:
        GLuint VAO, VBO, instanceVBO;
        size_t objectSize;

    public:
        Model(const float* points, size_t size);
        ~Model();
        
        void draw();
        void drawInstanced(GLuint instanceCount);
        void setupInstanceBuffer(const std::vector<glm::mat4>& transformations);
        
};
