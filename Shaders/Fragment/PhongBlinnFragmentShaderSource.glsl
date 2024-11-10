#version 330 core
#define MAX_LIGHTS 4

struct Light {
    vec3 position;
    vec3 color;
    float intensity;
};

in vec3 worldNorm;
in vec4 worldPos;
out vec4 out_Color;

uniform Light lights[MAX_LIGHTS];
uniform int numberOfLights;
uniform vec3 viewPosition;
uniform float shininess;

void main() {
    vec3 result = vec3(0.1, 0.1, 0.1); // Ambient component
    vec3 norm = normalize(worldNorm);
    vec3 viewDir = normalize(viewPosition - vec3(worldPos));

    for(int i = 0; i < numberOfLights; i++) {
        vec3 lightDir = normalize(lights[i].position - vec3(worldPos));
        float distance = length(lights[i].position - vec3(worldPos));

        // Attenuation calculation
        float constant = 1.0;
        float linear = 0.09;
        float quadratic = 0.032;
        float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

        // Diffuse component
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lights[i].color * lights[i].intensity * attenuation;

        // Specular component (Phong-Blinn)
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
        vec3 specular = spec * lights[i].color * lights[i].intensity * attenuation;

        // Add diffuse and specular components to the result
        result += diffuse + specular;
    }

    out_Color = vec4(result, 1.0);
}
