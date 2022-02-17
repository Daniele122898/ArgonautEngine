#version 460 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight;
uniform SpotLight spotLight;

#define POINT_LIGHT_COUNT 4
uniform PointLight pointLights[POINT_LIGHT_COUNT];

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    // diffuse light
    vec3 norm = normalize(Normal);
    // get vector from fragment to viewer
    vec3 viewDir = normalize(viewPos - FragPos);

    // Directional Light
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // Point Lights
    for (int i =0; i < POINT_LIGHT_COUNT; ++i) {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }
    // Spot light
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    // ambient
    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;
    // Get vector that points from fragment to light
    vec3 lightDir = normalize(-light.direction);
    // get angle between light dir and normal vec
    float diff = max(dot(normal, lightDir), 0.0);
    // multiply with lightcolor resulting in darker diffuse component
    // the greater the angle between the two vectors
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // specular highlight
    // reflect needs lightDir to be from light to fragment but its currently the other
    // way around so we negate
    vec3 reflectDir = reflect(-lightDir, normal);
    // the power is the shininess. More shininess = more reflective less diffusing
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 norm, vec3 fragPos, vec3 viewDir) {
    // ambient
    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;

    // diffuse light
    // Get vector that points from fragment to light
    vec3 lightDir = normalize(light.position - fragPos);
    // get angle between light dir and normal vec
    float diff = max(dot(norm, lightDir), 0.0);
    // multiply with lightcolor resulting in darker diffuse component
    // the greater the angle between the two vectors
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // specular highlight
    // reflect needs lightDir to be from light to fragment but its currently the other
    // way around so we negate
    vec3 reflectDir = reflect(-lightDir, norm);
    // the power is the shininess. More shininess = more reflective less diffusing
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    // point light
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 /
        (light.constant + light.linear * distance +
        light.quadratic * distance * distance);

    return attenuation * (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 norm, vec3 fragPos, vec3 viewDir) {
    // ambient
    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;
    // diffuse light
    // Get vector that points from fragment to light
    vec3 lightDir = normalize(light.position - fragPos);
    // spotlight
    // get angle between light dir and our fragment
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta-light.outerCutOff)/ epsilon, 0.0, 1.0);

    // get angle between light dir and normal vec
    float diff = max(dot(norm, lightDir), 0.0);
    // multiply with lightcolor resulting in darker diffuse component
    // the greater the angle between the two vectors
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // specular highlight
    // reflect needs lightDir to be from light to fragment but its currently the other
    // way around so we negate
    vec3 reflectDir = reflect(-lightDir, norm);
    // the power is the shininess. More shininess = more reflective less diffusing
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    // point light
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 /
        (light.constant + light.linear * distance +
        light.quadratic * distance * distance);

    return attenuation * intensity * (ambient + diffuse + specular);
}