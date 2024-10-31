#version 330
layout(location=0) in vec3 vp;  // Vertex position
layout(location=1) in vec3 vn;  // Vertex normal or color
uniform mat4 transformationMatrix;  // Transformation matrix
uniform mat4 viewMatrix;           // View matrix (from the camera)
uniform mat4 projectionMatrix;      // Projection matrix (perspective or orthographic)
out vec3 color;                     // Passing color to the fragment shader
void main() {
   gl_Position = projectionMatrix * viewMatrix * transformationMatrix * vec4(vp, 1.0);  // Combine matrices
   color = vn;  // Pass color to the fragment shader
}