#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

// Matrices for instanced rendering
layout(location = 2) in mat4 instanceMatrix;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 FragPos;
out vec3 Normal;

void main()
{
    mat4 modelMatrix = instanceMatrix;
    FragPos = vec3(modelMatrix * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(modelMatrix))) * normal;

    gl_Position = projectionMatrix * viewMatrix * vec4(FragPos, 1.0);
}
