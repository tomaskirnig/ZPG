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
#include "jehlan.h"
#include "skycube.h"

using namespace std;

class Model {
    private:
		GLuint VAO, VBO; // instanceVBO
        size_t objectSize;

    public:
		Model(const float* points, size_t size, int ver); 

        void Model0(const float* points, size_t size); // just obj coords
        void Model1(const float* points, size_t size); // obj coords and normals
        void Model2(const float* points, size_t size); // obj coords, normals and texture

        ~Model();
        
        void draw();   
};
