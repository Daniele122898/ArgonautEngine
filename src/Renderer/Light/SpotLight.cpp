//
// Created by Daniele on 2/20/2022.
//

#include "SpotLight.h"
namespace Argonaut {

    void SpotLight::UseLight(Shader &shader) {
        shader.setVec3("spotLight.position", m_position);
        shader.setVec3("spotLight.direction", m_direction);
        shader.setFloat("spotLight.cutOff", m_cutOffCos);
        shader.setFloat("spotLight.outerCutOff", m_outerCutOffCos);
        shader.setVec3("spotLight.ambient",  m_ambient);
        // darken diffuse light a bit
        shader.setVec3("spotLight.diffuse",  m_diffuse);
        shader.setVec3("spotLight.specular", m_specular);
        // attenuation
        shader.setFloat("spotLight.constant",  m_constant);
        shader.setFloat("spotLight.linear",    m_linear);
        shader.setFloat("spotLight.quadratic", m_quadratic);
    }

    void SpotLight::UpdateLight(Shader &shader, v3 ambient, v3 diffuse, v3 specular) {
        m_ambient = ambient;
        m_diffuse = diffuse;
        m_specular = specular;
        shader.setVec3("spotLight.ambient",  m_ambient);
        shader.setVec3("spotLight.diffuse",  m_diffuse);
        shader.setVec3("spotLight.specular", m_specular);
    }

    void SpotLight::UpdatePosition(Shader &shader, v3 position) {
        m_position = position;
        shader.setVec3("spotLight.position", m_position);
    }

    void SpotLight::UpdateDirection(Shader &shader, v3 direction) {
        m_direction = direction;
        shader.setVec3("spotLight.direction", m_direction);
    }

    void SpotLight::UpdateCutOffs(Shader &shader, float cutOff, float outerCutOff) {
        m_cutOffCos = glm::cos(glm::radians(cutOff));
        m_outerCutOffCos = glm::cos(glm::radians(outerCutOff));
        shader.setFloat("spotLight.cutOff", m_cutOffCos);
        shader.setFloat("spotLight.outerCutOff", m_outerCutOffCos);
    }

    void SpotLight::UpdateAttenuation(Shader &shader, float constant, float linear, float quadratic) {
        m_constant = constant;
        m_linear = linear;
        m_quadratic = quadratic;

        shader.setFloat("spotLight.constant",  m_constant);
        shader.setFloat("spotLight.linear",    m_linear);
        shader.setFloat("spotLight.quadratic", m_quadratic);
    }

    void SpotLight::DisableLight(Shader &shader) {
        v3 zero = {0.f, 0.f, 0.f};
        shader.setVec3("spotLight.ambient",  zero);
        shader.setVec3("spotLight.diffuse",  zero);
        shader.setVec3("spotLight.specular", zero);
    }
}