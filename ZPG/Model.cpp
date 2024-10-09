#include "Model.h"
using namespace std;

Model::Model(const float* points, size_t size) {
    numVertices = size / (sizeof(float) * 3);

    if (numVertices > 4) {
        glGenBuffers(1, &VBO); // generate the VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, size, points, GL_STATIC_DRAW);

        glGenVertexArrays(1, &VAO); //generate the VAO
        glBindVertexArray(VAO); //bind the VAO

        glEnableVertexAttribArray(0);  // Enable position attribute
        glEnableVertexAttribArray(1);  // Enable color attribute

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // Position attribute: location 0, 3 floats, stride of 6 floats
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)0);

        // Color attribute: location 1, 3 floats, offset by 3 floats
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
    }
    else /*if(numVertices <= 3)*/ {
        glGenBuffers(1, &VBO); // generate the VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, size, points, GL_STATIC_DRAW);

        glGenVertexArrays(1, &VAO); //generate the VAO
        glBindVertexArray(VAO); //bind the VAO
        glEnableVertexAttribArray(0); //enable vertex attributes
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    }
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
    else {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, numVertices);
        glBindVertexArray(0);
    }
}

void Model::deleteModel() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}