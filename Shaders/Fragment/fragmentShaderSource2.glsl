#version 330
in vec3 color;
out vec4 frag_colour;
void main () {
     frag_colour = vec4(color, 0.5); // Use the color passed from the vertex shader
}
