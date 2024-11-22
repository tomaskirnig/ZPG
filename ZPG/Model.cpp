#include "Model.h"

Model::Model(const float* points, size_t size) {
    objectSize = size / (sizeof(float) * 6); // 6 floats (3 position, 3 normal)

    glGenVertexArrays(1, &VAO); // generate the VAO
    glBindVertexArray(VAO); // bind the VAO

    glGenBuffers(1, &VBO); // generate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind the VBO
    glBufferData(GL_ARRAY_BUFFER, size, points, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);  // Enable position attribute

    // Position attribute: location 0, 3 floats, stride of 6 floats
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)0);

    glEnableVertexAttribArray(1);

    // Normal attribute: location 1, 3 floats, offset by 3 floats
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(3 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Model::Model(const float* points, size_t size, int i) {
    objectSize = size / (sizeof(float) * 8); // 8 floats (3 position, 3 normal, 2 texCoords)

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, points, GL_STATIC_DRAW);

    // Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture Coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}


// Delete the model
Model::~Model() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

// Draw the object
void Model::draw() {
    glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, objectSize); // Possible to use GL_TRIANGLE_STRIP
    glBindVertexArray(0);
}

//void Model::drawInstanced(GLuint instanceCount) {
//    glBindVertexArray(VAO);
//    glDrawArraysInstanced(GL_TRIANGLES, 0, objectSize, instanceCount);
//    glBindVertexArray(0);
//}

//void Model::setupInstanceBuffer(const std::vector<glm::mat4>& transformations) {
//    if (instanceVBO == 0) {
//        glGenBuffers(1, &instanceVBO);
//    }
//
//    glBindVertexArray(VAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
//    glBufferData(GL_ARRAY_BUFFER, transformations.size() * sizeof(glm::mat4), transformations.data(), GL_STATIC_DRAW);
//
//    // Set attribute pointers for matrix (4 times vec4)
//    GLsizei vec4Size = sizeof(glm::vec4);
//    for (int i = 0; i < 4; i++) {
//        glEnableVertexAttribArray(2 + i);
//        glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(vec4Size * i));
//        glVertexAttribDivisor(2 + i, 1);
//    }
//
//    glBindVertexArray(0);
//}

