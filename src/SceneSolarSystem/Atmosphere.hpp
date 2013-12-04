#ifndef ATMOSPHERE_HPP
#define ATMOSPHERE_HPP

#include "Commons.hpp"
#include "Planet.hpp"

class Atmosphere : public Planet {
    public:
		Atmosphere(const std::string& name, float planetRadius, float outerRadius);
        ~Atmosphere();

        void update(float deltaTime, float time);
        void draw() const;

        Model atmo;
    private:
        float time;
        float outerRadius;
        float innerRadius;
};

#endif // SUNHALO_HPP
