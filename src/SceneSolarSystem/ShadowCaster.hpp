#ifndef SHADOWCASTER_HPP
#define SHADOWCASTER_HPP

#include "Commons.hpp"

class ShadowCaster {
public:
    ShadowCaster();

    virtual void castShadow(const mat4f& MVP, const float& shadowBias) = 0;

protected:
    ShaderProgram* shadowProgram;
};

#endif // SHADOWCASTER_HPP
