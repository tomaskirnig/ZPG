#version 330 core
in vec3 worldNorm;
in vec4 worldPos;
out vec4 out_Color;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 viewPosition;  // Position of the viewer/camera
uniform float shininess;    // Shininess factor for specular reflection
void main() {
    // Normalize the world normal and calculate light direction
    vec3 norm = normalize(worldNorm);
    vec3 lightVector = normalize(lightPosition - vec3(worldPos));
    // Ambient component
    vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);
    // Diffuse component
    float diff = max(dot(norm, lightVector), 0.0);
    vec4 diffuse = vec4(diff * lightColor, 1.0);
    // Specular component (Phong)
    vec3 viewDir = normalize(viewPosition - vec3(worldPos));
    vec3 reflectDir = reflect(-lightVector, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec4 specular = vec4(spec * lightColor, 1.0);
    // Combine ambient, diffuse, and specular
    out_Color = ambient + diffuse + specular;
}