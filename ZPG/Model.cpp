#include "Model.h"

Model::Model(const float* points, size_t size, int ver) {
    switch (ver)
    {
    case 0: {
		Model0(points, size);
		break;
    }
	case 1: {
		Model1(points, size);
		break;
	}
	case 2: {
		Model2(points, size);
		break;
	}
    default:
		printf("Error: Model (constructor) version not found\n");
        break;
    }
}

// Delete the model
Model::~Model() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Model::Model0(const float* points, size_t size) {
    objectSize = size / (sizeof(float) * 3); // 3 floats (3 position)

    glGenVertexArrays(1, &VAO); // generate the VAO
    glGenBuffers(1, &VBO); // generate the VBO

    glBindVertexArray(VAO); // bind the VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind the VBO
    glBufferData(GL_ARRAY_BUFFER, size, points, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);  // Enable position attribute

    // Position attribute: location 0, 3 floats, stride of 3 floats
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Model::Model1(const float* points, size_t size) {
    objectSize = size / (sizeof(float) * 6); // 6 floats (3 position, 3 normal)

    glGenVertexArrays(1, &VAO); // generate the VAO
    glGenBuffers(1, &VBO); // generate the VBO

    glBindVertexArray(VAO); // bind the VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind the VBO
    glBufferData(GL_ARRAY_BUFFER, size, points, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);  // Enable position attribute
    glEnableVertexAttribArray(1);  // Enable normal attribute

    // Position attribute: location 0, 3 floats, stride of 6 floats
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)0);

    // Normal attribute: location 1, 3 floats, offset by 3 floats
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(3 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Model::Model2(const float* points, size_t size) {
    objectSize = size / (sizeof(float) * 8); // 8 floats (3 position, 3 normal, 2 texCoords)

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, points, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // Positions (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)0);

    // Normals (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(3 * sizeof(float)));

    // Texture Coordinates (location = 2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(6 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Draw the object
void Model::draw() {
    glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, objectSize); // Possible to use GL_TRIANGLE_STRIP
    glBindVertexArray(0);
}