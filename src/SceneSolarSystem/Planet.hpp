#ifndef PLANET_HPP
#define PLANET_HPP

#include "Commons.hpp"
#include "WorldObject.hpp"

class Planet : public WorldObject {
    public:
        Planet(const std::string& name, float radius, float orbRadius);
        ~Planet();

        virtual void update(float deltaTime) = 0;
        virtual void draw() const;

        float radius;
        float orbRadius;
        float orbSpeed;
        float rotSpeed;
        bool drawOrbit;
    protected:
        Model sphere;
        Model orbit;
};

#endif // PLANET_HPP
