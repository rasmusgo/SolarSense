#ifndef ORBITINGOBJECT_HPP
#define ORBITINGOBJECT_HPP

#include "Commons.hpp"
#include "WorldObject.hpp"

class Sun : public WorldObject {
    public:
        Sun();
        ~Sun();

        void update(float deltaTime);
        void draw() const;

        float radius;
    private:
        Model sph;

        float time;
};

#endif // ORBITINGOBJECT_HPP
