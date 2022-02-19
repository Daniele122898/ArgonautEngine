//
// Created by Daniele on 2/18/2022.
//

#include "PointLight.h"
#include <fmt/core.h>
namespace Argonaut {

    void PointLight::UseLight(Shader &shader, uint index) {
        std::string lstr = fmt::format("pointLights[{}].", index);
        shader.setVec3(lstr + "position", m_position);
        shader.setVec3(lstr + "ambient", m_ambient);
        shader.setVec3( lstr + "diffuse", m_diffuse);
        shader.setVec3(lstr + "specular", m_specular);
        shader.setFloat(lstr + "constant", m_constant);
        shader.setFloat(lstr + "linear", m_linear);
        shader.setFloat(lstr + "quadratic", m_quadratic);
    }

    void PointLight::UpdateLight(Shader &shader, uint index, v3 ambient, v3 diffuse, v3 specular) {
        this->m_ambient = ambient;
        this->m_diffuse = diffuse;
        this->m_specular = specular;

        std::string lstr = fmt::format("pointLights[{}].", index);
        shader.setVec3(lstr + "ambient", ambient);
        shader.setVec3( lstr + "diffuse", diffuse);
        shader.setVec3(lstr + "specular", specular);
    }

    void PointLight::UpdatePosition(Shader &shader, uint index, v3 position) {
        this->m_position = position;

        std::string lstr = fmt::format("pointLights[{}].", index);
        shader.setVec3(lstr + "position", position);
    }

    void PointLight::UpdateAttenuation(Shader& shader, uint index, float constant, float linear, float quadratic) {
        this->m_constant = constant;
        this->m_linear = linear;
        this->m_quadratic = quadratic;

        std::string lstr = fmt::format("pointLights[{}].", index);
        shader.setFloat(lstr + "constant", constant);
        shader.setFloat(lstr + "linear", linear);
        shader.setFloat(lstr + "quadratic", quadratic);
    }

    void PointLight::DisableLight(Shader &shader, uint index) {
        v3 zero = {0.f, 0.f, 0.f};
        std::string lstr = fmt::format("pointLights[{}].", index);
        shader.setVec3(lstr + "ambient", zero);
        shader.setVec3( lstr + "diffuse", zero);
        shader.setVec3(lstr + "specular", zero);
    }
}