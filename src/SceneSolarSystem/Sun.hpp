#ifndef ORBITINGOBJECT_HPP
#define ORBITINGOBJECT_HPP

#include "Commons.hpp"

class Sun : public GameObject {
    public:
        Sun();
        ~Sun();

        void update(float deltaTime);
        void draw() const;

        float radius;
    private:
        Model sph;//should be probably an abstract class so this would go out

        float time;
};

#endif // ORBITINGOBJECT_HPP
