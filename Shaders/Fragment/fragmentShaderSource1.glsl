#version 330
out vec4 frag_colour;
void main () {
     frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);
//     frag_colour = vec4 (0.5, 0.0, hello, 1.0); // Shader compilation error test
}