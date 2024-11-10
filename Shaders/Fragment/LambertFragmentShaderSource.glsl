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

void main() {
    // Normalize the world normal
    vec3 norm = normalize(worldNorm);

    // Ambient component
    vec3 ambient = vec3(0.1, 0.1, 0.1);

    // Initialize result color
    vec3 result = ambient;

    for(int i = 0; i < numberOfLights; i++) {
        // Light direction and distance
        vec3 lightDir = normalize(lights[i].position - vec3(worldPos));
        float distance = length(lights[i].position - vec3(worldPos));

        // Attenuation calculation
        float constant = 1.0;
        float linear = 0.09;
        float quadratic = 0.032;
        float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

        // Diffuse component
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lights[i].color * lights[i].intensity;

        // Apply attenuation to diffuse component
        diffuse *= attenuation;

        // Accumulate contributions
        result += diffuse;
    }

    out_Color = vec4(result, 1.0);
}
