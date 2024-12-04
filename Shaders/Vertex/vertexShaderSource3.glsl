#version 330
layout(location = 0) in vec3 vp; // Vertex position
layout(location = 1) in vec3 vn; // Vertex normal

out vec3 worldNorm;
out vec4 worldPos;

uniform mat4 transformationMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {
    worldPos = transformationMatrix * vec4(vp, 1.0);
    mat3 normalMatrix = transpose(inverse(mat3(transformationMatrix / transformationMatrix[3][3])));
    worldNorm = normalize(normalMatrix * vn);
    gl_Position = projectionMatrix * viewMatrix * worldPos;
}