//
// Created by Daniele on 2/18/2022.
//

#ifndef ENGINE_LIGHT_H
#define ENGINE_LIGHT_H

#include "../../Base/Types.h"

namespace Argonaut {
    class Light {
    public:
        Light(v3 ambient, v3 diffuse, v3 specular):
                m_ambient{ambient}, m_diffuse{diffuse}, m_specular{specular}{}

    protected:
        v3 m_ambient;
        v3 m_diffuse;
        v3 m_specular;
    };
}

#endif //ENGINE_LIGHT_H
