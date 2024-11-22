#version 330 core

struct Material {
    sampler2D diffuseTexture;
    vec3 ambient;	
    vec3 diffuse;
    vec3 specular;	
    float shininess;
    bool hasTexture;
};

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
in vec2 TexCoords;
out vec4 out_Color;

uniform Material material; 
uniform DirectionalLight dirLights[MAX_DIR_LIGHTS];
uniform int numberOfDirLights;

uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int numberOfPointLights;

uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
uniform int numberOfSpotLights;

uniform vec3 viewPosition;

void main() {
    //Ambient
    vec3 ambientColor = material.ambient;

    //Diffuse 
    vec3 norm = normalize(worldNorm);
    vec3 viewDir = normalize(viewPosition - vec3(worldPos));
    
    vec3 diffuseColor;

    if (material.hasTexture) {
        diffuseColor = texture(material.diffuseTexture, TexCoords).rgb;
    } else {
        diffuseColor = material.diffuse;
    }
 
    vec3 result = material.ambient * diffuseColor;

    // Directional lights
    for (int i = 0; i < numberOfDirLights; ++i) {
        vec3 lightDir = normalize(-dirLights[i].direction);

        // Diffuse component
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * diffuseColor * dirLights[i].color * dirLights[i].intensity;

        // Specular component
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = spec * material.specular * dirLights[i].color * dirLights[i].intensity;

        result += diffuse + specular;
    }

    // Point lights
    for (int i = 0; i < numberOfPointLights; ++i) {
        vec3 lightDir = normalize(pointLights[i].position - vec3(worldPos));
        float distance = length(pointLights[i].position - vec3(worldPos));

        // Attenuation
        float attenuation = pointLights[i].intensity / (distance * distance);

        // Diffuse component
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * diffuseColor * pointLights[i].color * attenuation;

        // Specular component
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = spec * material.specular * pointLights[i].color * attenuation;

        result += diffuse + specular;
    }

    // Spotlights
    for (int i = 0; i < numberOfSpotLights; ++i) {
        vec3 lightDir = normalize(spotLights[i].position - vec3(worldPos));

        // Spotlight intensity based on angle
        float theta = dot(lightDir, normalize(-spotLights[i].direction));
        float epsilon = spotLights[i].cutOff - spotLights[i].outerCutOff;
        float spotlightIntensity = clamp((theta - spotLights[i].outerCutOff) / epsilon, 0.0, 1.0);

        // Diffuse component
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * diffuseColor * spotLights[i].color * spotlightIntensity;

        // Specular component
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = spec * material.specular * spotLights[i].color * spotlightIntensity;

        result += diffuse + specular;
    }

    out_Color = texture(material.diffuseTexture, TexCoords);//result
}