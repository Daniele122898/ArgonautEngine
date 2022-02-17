#version 460 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;

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
uniform Light light;

void main()
{
    // ambient
    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;

    // diffuse light
    vec3 norm = normalize(Normal);
    // Get vector that points from fragment to light
    vec3 lightDir = normalize(light.position - FragPos);
    // get angle between light dir and normal vec
    float diff = max(dot(norm, lightDir), 0.0);
    // multiply with lightcolor resulting in darker diffuse component
    // the greater the angle between the two vectors
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // specular highlight
    // get vector from fragment to viewer
    vec3 viewDir = normalize(viewPos - FragPos);
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

    vec3 result = attenuation * (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}