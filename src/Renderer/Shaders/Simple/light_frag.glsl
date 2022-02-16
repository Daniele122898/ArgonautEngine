#version 460 core
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    float ambientStrength = 0.1;
    float specularStrength = 0.5;

    // ambient
    vec3 ambient = ambientStrength * lightColor;

    // diffuse light
    vec3 norm = normalize(Normal);
    // Get vector that points from fragment to light
    vec3 lightDir = normalize(lightPos - FragPos);
    // get angle between light dir and normal vec
    float diff = max(dot(norm, lightDir), 0.0);
    // multiply with lightcolor resulting in darker diffuse component
    // the greater the angle between the two vectors
    vec3 diffuse = diff*lightColor;

    // specular highlight
    // get vector from fragment to viewer
    vec3 viewDir = normalize(viewPos - FragPos);
    // reflect needs lightDir to be from light to fragment but its currently the other
    // way around so we negate
    vec3 reflectDir = reflect(-lightDir, norm);
    // the power is the shininess. More shininess = more reflective less diffusing
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse+ specular) * objectColor;
    FragColor = vec4(result, 1.0);
}