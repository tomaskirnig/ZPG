#version 330 core

layout(location = 0) in vec3 vp; // Vertex position
layout(location = 1) in vec3 vn; // Vertex normal
layout(location = 2) in vec2 vt; // Texture coordinates

out vec3 worldNorm;
out vec4 worldPos;
out vec2 TexCoords;

uniform mat4 transformationMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {
    worldPos = transformationMatrix * vec4(vp, 1.0);
    mat3 normalMatrix = transpose(inverse(mat3(transformationMatrix)));
    worldNorm = normalize(normalMatrix * vn);
    TexCoords = vt;
    gl_Position = projectionMatrix * viewMatrix * worldPos;
}
