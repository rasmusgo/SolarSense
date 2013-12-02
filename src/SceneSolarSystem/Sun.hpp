#ifndef ORBITINGOBJECT_HPP
#define ORBITINGOBJECT_HPP

#include "Commons.hpp"
#include "Planet.hpp"

class Sun : public Planet {
    public:
        Sun(const std::string& name, float radius);
        ~Sun();

        void update(float deltaTime, float time);
        void draw() const;

    private:
        float time;
};

#endif // ORBITINGOBJECT_HPP
