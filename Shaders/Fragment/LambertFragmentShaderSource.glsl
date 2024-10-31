#version 330 core
in vec3 worldNorm;
in vec4 worldPos;
out vec4 out_Color;
uniform vec3 lightPosition;
uniform vec3 lightColor;
void main() {
    vec3 lightVector = normalize(lightPosition - vec3(worldPos)); // Light direction
    float diff = max(dot(normalize(worldNorm), lightVector), 0.0); // Diffuse lighting
    vec4 diffuse = vec4(diff * lightColor, 1.0);
    vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0); // Ambient lighting
    out_Color = ambient + diffuse;
}