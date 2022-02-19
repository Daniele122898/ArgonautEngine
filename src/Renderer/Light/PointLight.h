//
// Created by Daniele on 2/18/2022.
//

#ifndef ENGINE_POINTLIGHT_H
#define ENGINE_POINTLIGHT_H

#include "../../Base/Types.h"
#include "Light.h"
#include "../Shader.h"

namespace Argonaut {
    class PointLight : public Light {
    public:
        PointLight(v3 ambient, v3 diffuse, v3 specular, v3 position, float constant,
                   float linear, float quadratic) : Light{ambient, diffuse, specular},
                                                    m_position{position}, m_constant{constant}, m_linear{linear}, m_quadratic{quadratic} {}

        void UseLight(Shader& shader, uint index);
        void UpdateLight(Shader& shader, uint index, v3 ambient, v3 diffuse, v3 specular);
        void UpdatePosition(Shader& shader, uint index, v3 position);
        void UpdateAttenuation(Shader& shader, uint index, float constant,float linear, float quadratic);
        void DisableLight(Shader& shader, uint index);


    protected:
        v3 m_position;
        float m_constant;
        float m_linear;
        float m_quadratic;
    };
}


#endif //ENGINE_POINTLIGHT_H
