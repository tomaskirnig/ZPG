#version 330
in vec3 color;     // Receive the interpolated color from the vertex shader
out vec4 frag_colour;  // Declare the output color variable
void main() {
    frag_colour = vec4(color, 1.0);  // Set the fragment color using the input color and alpha = 1.0
}