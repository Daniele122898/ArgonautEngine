#version 460 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform Light light;
uniform Material material;
uniform vec3 viewPos;

void main()
{
    // ambient
    vec3 ambient = material.ambient * light.ambient;

    // diffuse light
    vec3 norm = normalize(Normal);
    // Get vector that points from fragment to light
    vec3 lightDir = normalize(light.position - FragPos);
    // get angle between light dir and normal vec
    float diff = max(dot(norm, lightDir), 0.0);
    // multiply with lightcolor resulting in darker diffuse component
    // the greater the angle between the two vectors
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // specular highlight
    // get vector from fragment to viewer
    vec3 viewDir = normalize(viewPos - FragPos);
    // reflect needs lightDir to be from light to fragment but its currently the other
    // way around so we negate
    vec3 reflectDir = reflect(-lightDir, norm);
    // the power is the shininess. More shininess = more reflective less diffusing
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}