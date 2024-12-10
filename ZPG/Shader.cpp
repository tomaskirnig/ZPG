#include "Shader.h"
#include "SpotLight.h"
#include "DirectionalLight.h"
#include "PointLight.h"

Shader::Shader(string vertexFile, string fragmentFile) {
	this->shininess = shininess;

	cout << "Shininess" << this->shininess << endl;

    vertexFile = "../Shaders/Vertex/" + vertexFile + ".glsl";
	fragmentFile = "../Shaders/Fragment/" + fragmentFile + ".glsl";

	ID = loadShader(vertexFile.c_str(), fragmentFile.c_str());
}

// Compile a shader from source code
GLuint Shader::compileShader(const char* source, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    checkCompileErrors(shader, (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT");
    return shader;
}

// Check for compilation errors
void Shader::checkCompileErrors(GLuint shader, string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << endl;
            exit(EXIT_FAILURE);
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << endl;
            exit(EXIT_FAILURE);
        }
    }
}

void Shader::update(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const std::vector<LightData>& lights, const glm::vec3& viewPosition) {
    use();

    glUniformMatrix4fv(getUniformLocation("viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(getUniformLocation("projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    glUniform3fv(getUniformLocation("viewPosition"), 1, glm::value_ptr(viewPosition));

    // Separate lights by type
    std::vector<LightData> pointLights;
    std::vector<LightData> dirLights;
    std::vector<LightData> spotLights;

    for (const auto& light : lights) {
        switch (light.type) {
        case LightType::POINT:
            pointLights.push_back(light);
            break;
        case LightType::DIRECTIONAL:
            dirLights.push_back(light);
            break;
        case LightType::SPOTLIGHT:
            spotLights.push_back(light);
            break;
        default:
            std::cerr << "Unknown light type detected!" << std::endl;
        }
    }

    setPointLights(pointLights);

    setDirLights(dirLights);

    setSpotLights(spotLights);

    glUniform1f(getUniformLocation("shininess"), shininess);

    // Debug information
    /*std::cout << "Number of Point Lights: " << pointLights.size() << std::endl;
    std::cout << "Number of Dir Lights: " << dirLights.size() << std::endl;
    std::cout << "Number of Spot Lights: " << spotLights.size() << std::endl;

    for (size_t i = 0; i < pointLights.size(); ++i) {
        std::cout << "Point Light [" << i << "] Position: "
            << pointLights[i].position.x << ", "
            << pointLights[i].position.y << ", "
            << pointLights[i].position.z << std::endl;
        std::cout << "Point Light [" << i << "] Color: "
            << pointLights[i].color.r << ", "
            << pointLights[i].color.g << ", "
            << pointLights[i].color.b << std::endl;
        std::cout << "Point Light [" << i << "] Intensity: " << pointLights[i].intensity << std::endl;
    }*/
}


// Retrieve the location of a uniform, caching it after the first retrieval
GLint Shader::getUniformLocation(const string& name) {
    // If the uniform location is not in the cache, get it from the shader and cache it
    if (uniformLocations.find(name) == uniformLocations.end()) {
        uniformLocations[name] = glGetUniformLocation(ID, name.c_str());
    }
    // Return the cached location
    return uniformLocations[name];
}


void Shader::setSpotLights(const std::vector<LightData>& spotLights) {
    int spotLightCount = static_cast<int>(spotLights.size());
    glUniform1i(getUniformLocation("numberOfSpotLights"), spotLightCount);

    for (int i = 0; i < spotLightCount && i < MAX_SPOT_LIGHTS; ++i) {
        std::string index = std::to_string(i);

        glUniform3fv(getUniformLocation("spotLights[" + index + "].position"), 1, glm::value_ptr(spotLights[i].position));
        glUniform3fv(getUniformLocation("spotLights[" + index + "].direction"), 1, glm::value_ptr(spotLights[i].direction));
        glUniform3fv(getUniformLocation("spotLights[" + index + "].color"), 1, glm::value_ptr(spotLights[i].color));
        glUniform1f(getUniformLocation("spotLights[" + index + "].intensity"), spotLights[i].intensity);
        glUniform1f(getUniformLocation("spotLights[" + index + "].cutOff"), glm::cos(glm::radians(spotLights[i].cutOff)));
        glUniform1f(getUniformLocation("spotLights[" + index + "].outerCutOff"), glm::cos(glm::radians(spotLights[i].outerCutOff)));
    }
}

void Shader::setPointLights(const std::vector<LightData>& pointLights) {
    int pointLightCount = static_cast<int>(pointLights.size());
    glUniform1i(getUniformLocation("numberOfPointLights"), pointLightCount);

    for (int i = 0; i < pointLightCount && i < MAX_POINT_LIGHTS; ++i) {
        std::string index = std::to_string(i);

        glUniform3fv(getUniformLocation("pointLights[" + index + "].position"), 1, glm::value_ptr(pointLights[i].position));
        glUniform3fv(getUniformLocation("pointLights[" + index + "].color"), 1, glm::value_ptr(pointLights[i].color));
        glUniform1f(getUniformLocation("pointLights[" + index + "].intensity"), pointLights[i].intensity);
    }
}

void Shader::setDirLights(const std::vector<LightData>& dirLights) {
    int dirLightCount = static_cast<int>(dirLights.size());
    glUniform1i(getUniformLocation("numberOfDirLights"), dirLightCount);

    for (int i = 0; i < dirLightCount && i < MAX_DIR_LIGHTS; ++i) {
        std::string index = std::to_string(i);

        glUniform3fv(getUniformLocation("dirLights[" + index + "].direction"), 1, glm::value_ptr(dirLights[i].direction));
        glUniform3fv(getUniformLocation("dirLights[" + index + "].color"), 1, glm::value_ptr(dirLights[i].color));
        glUniform1f(getUniformLocation("dirLights[" + index + "].intensity"), dirLights[i].intensity);
    }
}

void Shader::setMaterial(const Material* material) {
    glUniform3fv(getUniformLocation("material.ambient"), 1, glm::value_ptr(material->getAmbient()));
    glUniform3fv(getUniformLocation("material.diffuse"), 1, glm::value_ptr(material->getDiffuse()));
    glUniform3fv(getUniformLocation("material.specular"), 1, glm::value_ptr(material->getSpecular()));
    glUniform1f(getUniformLocation("material.shininess"), material->getShininess());

    if (material->getTexture() != nullptr){
        // Bind and set textures
        material->bindTexture(material->getTextureUnit());
        if (material->getTextureType() == TextureType::Texture2D) {
            glUniform1i(getUniformLocation("textureUnitID"), material->getTextureUnit());
            glUniform1i(getUniformLocation("material.hasTexture"), 1);
        }
        else {
			glUniform1i(getUniformLocation("UISky"), material->getTextureUnit());
			//cout << "UISky: " << material->getTextureUnit() << endl;
        }
	}
	else {
		glUniform1i(getUniformLocation("material.hasTexture"), 0);
	}
}


