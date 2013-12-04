#ifndef PLANET_HPP
#define PLANET_HPP

#include "Commons.hpp"
#include "WorldObject.hpp"
#include "Orbit.hpp"

// Must be set son of a WorldObject

class Planet : public WorldObject {
    public:
        Planet(const std::string& name, float radius, float orbRadius);
        ~Planet();

        virtual void update(float deltaTime) = 0;
        virtual void draw() const = 0;

        float radius;
        float orbRadius;
        float orbSpeed;
        float rotSpeed;
        float tilt;
        bool drawOrbit;

        const GameObject* getArroundParent() const;
    protected:
        Model sphere;

    private:
        Orbit* orbit;
};

#endif // PLANET_HPP
