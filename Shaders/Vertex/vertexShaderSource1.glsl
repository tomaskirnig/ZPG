#version 330
layout(location=0) in vec3 vp; // Position attribute
layout(location=1) in vec3 vn;  // Color attribute
out vec3 color;                 // Passing color to the fragment shader
void main () {
   gl_Position = vec4 (vp, 1.0);
   color = vn;                // Assigning input color to the output
}