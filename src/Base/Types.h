//
// Created by Daniele on 22/07/2021.
//

#ifndef ENGINE_TYPES_H
#define ENGINE_TYPES_H

#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace Argonaut {
    // Unit types
    typedef unsigned int uint;

    typedef glm::vec4 v4;
    typedef glm::vec3 v3;
    typedef glm::vec2 v2;
    typedef glm::mat4 m4;


    typedef std::string string;
    template<class T>
    using vec = std::vector<T>;
}
#endif //ENGINE_TYPES_H
