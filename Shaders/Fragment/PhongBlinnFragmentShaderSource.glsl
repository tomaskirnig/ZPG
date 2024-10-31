#version 330 core
in vec3 worldNorm;
in vec4 worldPos;
out vec4 out_Color;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 viewPosition;
uniform float shininess;
void main() {
    vec3 norm = normalize(worldNorm);
    vec3 lightVector = normalize(lightPosition - vec3(worldPos));
    // Ambient
    vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);
    // Diffuse
    float diff = max(dot(norm, lightVector), 0.0);
    vec4 diffuse = vec4(diff * lightColor, 1.0);
    // Specular (Blinn-Phong)
    vec3 viewDir = normalize(viewPosition - vec3(worldPos));
    vec3 halfwayDir = normalize(lightVector + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
    vec4 specular = vec4(spec * lightColor, 1.0);
    // Combine
    out_Color = ambient + diffuse + specular;
}