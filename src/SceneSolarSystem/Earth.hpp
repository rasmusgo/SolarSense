#ifndef EARTH_HPP
#define EARTH_HPP

#include "Planet.hpp"
#include "Atmosphere.hpp"

class Earth : public Planet {
    public:
        Earth(const std::string& name, float radius, float orbRadius);
        ~Earth();

        void update(float deltaTime, float time);
        void draw() const;

        Model atmo;
    private:
        float innerRadius;
        float outerRadius;
        //vec3f rotationAxis;
        mat4f axisRotation;
};

#endif // EARTH_HPP
