#ifndef PLANET_HPP
#define PLANET_HPP

#include "Commons.hpp"
#include "WorldObject.hpp"
#include "ShadowCaster.hpp"

class Planet : public WorldObject, public ShadowCaster {
    public:
        Planet(const std::string& name, float radius, float orbRadius);
        ~Planet();

        virtual void update(float deltaTime) = 0;
        virtual void draw() const;

        void castShadow(const mat4f &MVP, const float& shadowBias);

        float radius;
        float orbRadius;
        float orbSpeed;
        float rotSpeed;
        bool drawOrbit;
    protected:
        Model sphere;
        Model orbit;
        Model shadow;
};

#endif // PLANET_HPP
