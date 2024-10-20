#include "Model.h"

Model::Model(const float* points, size_t size) {
    objectSize = size;
    int numVertices = objectSize / (sizeof(float) * 6);

    glGenBuffers(1, &VBO); // generate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind the VBO
    glBufferData(GL_ARRAY_BUFFER, size, points, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO); // generate the VAO
    glBindVertexArray(VAO); // bind the VAO

    glEnableVertexAttribArray(0);  // Enable position attribute
    glEnableVertexAttribArray(1);  // Enable color attribute

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Position attribute: location 0, 3 floats, stride of 6 floats
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)0);

    // Color attribute: location 1, 3 floats, offset by 3 floats
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
}

// Draw the object
void Model::draw() {
    glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, objectSize); // Possible to use GL_TRIANGLE_STRIP
    glBindVertexArray(0);
}

// Delete the model
void Model::deleteModel() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}