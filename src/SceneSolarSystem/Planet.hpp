#ifndef PLANET_HPP
#define PLANET_HPP

#include "Commons.hpp"

class Planet : public GameObject {
    public:
        Planet(vec3f scale, float orbRadius, float orbSpeed);
        ~Planet();

        virtual void update(float deltaTime) = 0;
        virtual void draw() const = 0;
        virtual void drawFrom(mat4f from) const = 0;


    protected:
        float orbSpeed;
        float orbRadius;
        Model sphere;//should be probably an abstract class so this would go out
        Model orbit;
        mat4f baseMatrix;
        float timeAcc;

    friend class OrbitingObject;
    friend class Earth;
};

#endif // PLANET_HPP
