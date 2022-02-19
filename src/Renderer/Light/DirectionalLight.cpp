//
// Created by Daniele on 2/18/2022.
//

#include "DirectionalLight.h"
namespace Argonaut {

    void DirectionalLight::UseLight(Shader &shader) {
        shader.setVec3("dirLight.direction", m_direction);
        shader.setVec3("dirLight.ambient", m_ambient);
        shader.setVec3("dirLight.diffuse", m_diffuse);
        shader.setVec3("dirLight.specular", m_specular);
    }

    void DirectionalLight::DisableLight(Shader &shader) {
        v3 zero = {0.f, 0.f, 0.f};
        shader.setVec3("dirLight.ambient",  zero);
        shader.setVec3("dirLight.diffuse",  zero);
        shader.setVec3("dirLight.specular", zero);
    }

    void DirectionalLight::UpdateAmbient(Shader &shader, v3 ambient) {
        this->m_ambient = ambient;
        shader.setVec3("dirLight.ambient",  ambient);
    }

    void DirectionalLight::UpdateDiffuse(Shader &shader, v3 diffuse) {
        this->m_diffuse = diffuse;
        shader.setVec3("dirLight.diffuse",  diffuse);
    }

    void DirectionalLight::UpdateSpecular(Shader &shader, v3 specular) {
        this->m_specular;
        shader.setVec3("dirLight.specular", specular);
    }

    void DirectionalLight::UpdateDirection(Shader &shader, v3 dir) {
        m_direction = dir;
        shader.setVec3("dirLight.direction", m_direction);
    }
}