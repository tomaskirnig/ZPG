#version 330 core
#define MAX_LIGHTS 4

struct Light {
    vec3 position;
    vec3 color;
    vec3 intensity;
};

in vec3 worldNorm;
in vec4 worldPos;
out vec4 out_Color;

uniform Light lights[MAX_LIGHTS];
uniform int numberOfLights;
uniform vec3 viewPosition;

void main() {
    vec3 result = vec3(0.1, 0.1, 0.1); // Ambient component
    vec3 norm = normalize(worldNorm);
    vec3 viewDir = normalize(viewPosition - vec3(worldPos));

    for(int i = 0; i < numberOfLights; i++) {
        vec3 lightDir = normalize(lights[i].position - vec3(worldPos));
        float diff = max(dot(norm, lightDir), 0.0); // Diffuse component
        vec3 diffuse = diff * lights[i].color;
        
        // Specular component (Phong-Blinn)
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(norm, halfwayDir), 0.0), 128.0);
        vec3 specular = spec * lights[i].color;

        result += diffuse + specular;
    }

    out_Color = vec4(result, 1.0);
}
