#pragma once
#include <GL/glew.h>
#include <cstddef>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

//Include objects
#include "sphere.h"
#include "tree.h"
#include "bushes.h"
#include "plain.h"
#include "suzi_flat.h"
#include "suzi_smooth.h"
#include "plain_texture.h"

using namespace std;

class Model {
    private:
		GLuint VAO, VBO; // instanceVBO
        size_t objectSize;

    public:
        Model(const float* points, size_t size); // without texture
		Model(const float* points, size_t size, int i); // with texture
        ~Model();
        
        void draw();
        /*void drawInstanced(GLuint instanceCount);
        void setupInstanceBuffer(const std::vector<glm::mat4>& transformations);*/
        
};
