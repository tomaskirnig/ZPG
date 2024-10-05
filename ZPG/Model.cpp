#include "Model.h"


Model::Model(float* points, size_t size) {
    numVertices = size / (sizeof(float) * 3);
    
    glGenBuffers(1, &VBO); // generate the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, points, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO); //generate the VAO
    glBindVertexArray(VAO); //bind the VAO
    glEnableVertexAttribArray(0); //enable vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

void Model::draw() {
    if (numVertices == 3) {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, numVertices);
        glBindVertexArray(0);
    }
    else if (numVertices == 4) {
        glBindVertexArray(VAO);
        glDrawArrays(GL_QUADS, 0, numVertices);
        glBindVertexArray(0);
    }
}

void Model::deleteModel() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}