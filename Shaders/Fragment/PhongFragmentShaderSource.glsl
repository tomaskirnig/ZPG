// Phong_MultiLightFragmentShader.glsl
#version 330 core
#define MAX_POINT_LIGHTS 4
#define MAX_DIR_LIGHTS 4
#define MAX_SPOT_LIGHTS 4

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
};

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float intensity;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    float cutOff;
    float outerCutOff;
};

in vec3 worldNorm;
in vec4 worldPos;
out vec4 out_Color;


uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int numberOfPointLights;

uniform DirectionalLight dirLights[MAX_DIR_LIGHTS];
uniform int numberOfDirLights;

uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
uniform int numberOfSpotLights;

uniform vec3 viewPosition;  // Position of the viewer/camera
uniform float shininess;    // Shininess factor for specular reflection

void main() {
    // Normalize the world normal
    vec3 norm = normalize(worldNorm);
    vec3 fragPos = vec3(worldPos);
    vec3 viewDir = normalize(viewPosition - fragPos);

    // Initialize result color
    vec3 result = vec3(0.1, 0.1, 0.1);

    // Directional lights
    for (int i = 0; i < numberOfDirLights; ++i) {
        vec3 lightDir = normalize(-dirLights[i].direction);
        // Diffuse
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * dirLights[i].color * dirLights[i].intensity;

        // Specular (Phong)
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = spec * dirLights[i].color * dirLights[i].intensity;

        result += diffuse + specular;
    }

    //Point lights
    for(int i = 0; i < numberOfPointLights; i++) {
        // Light direction and distance
        vec3 lightDir = normalize(pointLights[i].position - fragPos);
        float distance = length(pointLights[i].position - fragPos);

        // Attenuation factors
        float constant = 1.0;
        float linear = 0.09;
        float quadratic = 0.032;
        float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));


        // Diffuse component
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * pointLights[i].color * pointLights[i].intensity;

        // Specular component (Phong)
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = spec * pointLights[i].color * pointLights[i].intensity;

        // Apply attenuation
        diffuse *= attenuation;
        specular *= attenuation;

        // Accumulate contributions
        result += diffuse + specular;
    }
    
    // Spotlights
    for(int i = 0; i < numberOfSpotLights; i++) {
        // Light direction and distance
        vec3 lightDir = normalize(spotLights[i].position - fragPos);
        float distance = length(spotLights[i].position - fragPos);

        // Attenuation factors
        float constant = 1.0;
        float linear = 0.09;
        float quadratic = 0.032;
        float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));


        // Diffuse component
        float diff = max(dot(norm, lightDir), 0.0);

	// Spotlight intensity based on angle
        float theta = dot(lightDir, normalize(-spotLights[i].direction));
        float epsilon = spotLights[i].cutOff - spotLights[i].outerCutOff;
	float intensity = clamp((theta - spotLights[i].outerCutOff) / epsilon, 0.0, 1.0);

        vec3 diffuse = diff * spotLights[i].color * intensity;

	// Specular component (Phong)
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = spec * spotLights[i].color * spotLights[i].intensity;

        // Apply attenuation
        //diffuse *= attenuation;
        //specular *= attenuation;

        // Accumulate contributions
        result += diffuse + specular;
    }

    out_Color = vec4(result, 1.0);
}
