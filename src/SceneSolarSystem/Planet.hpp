#ifndef PLANET_HPP
#define PLANET_HPP

#include "Commons.hpp"
#include "WorldObject.hpp"

class Planet : public WorldObject {
    public:
        Planet(const std::string& name);
        ~Planet();

        virtual void update(float deltaTime) = 0;
        virtual void draw() const;

        float radius;
        float orbSpeed;
        float orbRadius;
        bool drawOrbit;
    protected:
        Model sphere;//should be probably an abstract class so this would go out
        Model orbit;
};

#endif // PLANET_HPP
