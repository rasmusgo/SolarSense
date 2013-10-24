#ifndef COMMONS_HPP
#define COMMONS_HPP

#include "VBE/includes.hpp"
#include "glm/gtc/quaternion.hpp"  // Quaternions

typedef glm::detail::tquat<float> quat;
typedef glm::detail::tquat<double> dquat;

namespace Utils {
    inline vec3f posFromMatrix(mat4f& m) {
        return vec3f(m[3][0],m[3][1],m[3][2]);
    }

    inline vec3f lerp(vec3f& from, vec3f& to, float t) {
        return to*t + from*(1.0f-t) ;
    }
}

#endif // COMMONS_HPP
