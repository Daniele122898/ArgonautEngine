#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalM;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);

    // Calculate fragpos by multiplying vertex pos by model matrix to get
    // world space coordinates
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = normalM * aNormal;
}