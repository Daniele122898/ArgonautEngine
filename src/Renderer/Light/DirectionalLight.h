//
// Created by Daniele on 2/18/2022.
//

#ifndef ENGINE_DIRECTIONALLIGHT_H
#define ENGINE_DIRECTIONALLIGHT_H

#include "../../Base/Types.h"
#include "Light.h"
#include "../Shader.h"

namespace Argonaut {
    class DirectionalLight : public Light{
    public:
        DirectionalLight(v3 ambient, v3 diffuse, v3 specular, v3 direction)
        : Light{ambient, diffuse, specular}, m_direction{direction} {}

        void UseLight(Shader& shader);
        void DisableLight(Shader& shader);
        void UpdateAmbient(Shader& shader, v3 ambient);
        void UpdateDiffuse(Shader& shader, v3 diffuse);
        void UpdateSpecular(Shader& shader, v3 specular);
        void UpdateDirection(Shader& shader, v3 dir);

    protected:
        v3 m_direction;
    };
}


#endif //ENGINE_DIRECTIONALLIGHT_H
