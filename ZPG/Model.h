#pragma once
#include <GL/glew.h>
#include <cstddef>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include<assimp/Importer.hpp>	// C++ importerinterface
#include<assimp/scene.h>		// aiSceneoutputdata structure
#include<assimp/postprocess.h>	// Post processingflags

#include "DrawType.h"

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
#include "building.h"
#include "skydome.h"

#define OBJ_MODEL_PATH "../Models/Models/"

using namespace std;

struct Vertex
{
    float Position[3];
    float Normal[3];
    float Texture[2];
    float Tangent[3];
};



class Model {
    private:
		GLuint VAO, VBO; // instanceVBO
        size_t objectSize;
        DrawType drawType;

    public:
        Model(const float* points, size_t size, int ver);
        Model(std::string fileName);

        void Model0(const float* points, size_t size); // just obj coords
        void Model1(const float* points, size_t size); // obj coords and normals
        void Model2(const float* points, size_t size); // obj coords, normals and texture

        ~Model();
        
        void draw();   
};
