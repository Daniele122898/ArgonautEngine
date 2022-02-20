//
// Created by Daniele on 2/20/2022.
//

#ifndef ENGINE_SPOTLIGHT_H
#define ENGINE_SPOTLIGHT_H

#include "../../Base/Types.h"
#include "Light.h"
#include "../Shader.h"
#include <glm/glm.hpp>

namespace Argonaut {
    class SpotLight : public Light {
    public:
        SpotLight(v3 ambient, v3 diffuse, v3 specular, v3 position, v3 direction,
                  float cutOff, float outerCutOff, float constant, float linear, float quadratic)
                  : Light{ambient, diffuse, specular}, m_position{position}, m_direction{direction},
                    m_cutOffCos{glm::cos(glm::radians(cutOff))}, m_outerCutOffCos{glm::cos(glm::radians(outerCutOff))},
                    m_constant{constant}, m_linear{linear}, m_quadratic{quadratic} {}

        void UseLight(Shader& shader);
        void UpdateLight(Shader& shader, v3 ambient, v3 diffuse, v3 specular);
        void UpdatePosition(Shader& shader, v3 position);
        void UpdateDirection(Shader& shader, v3 direction);
        void UpdateCutOffs(Shader& shader, float cutOff, float outerCutOff);
        void UpdateAttenuation(Shader& shader, float constant,float linear, float quadratic);
        void DisableLight(Shader& shader);


    protected:
        v3 m_position;
        v3 m_direction;
        float m_cutOffCos;
        float m_outerCutOffCos;
        float m_constant;
        float m_linear;
        float m_quadratic;
    };
}

#endif //ENGINE_SPOTLIGHT_H
