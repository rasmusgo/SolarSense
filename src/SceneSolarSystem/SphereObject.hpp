#ifndef SPHEREOBJECT_HPP
#define SPHEREOBJECT_HPP

#include "Commons.hpp"

class SphereObject : public GameObject {
    public:
        SphereObject();
        virtual ~SphereObject();

        void update(float deltaTime, float time);
        void updateMatrix();
        void draw() const;

        float radius;
    private:
        Model sph;
};


#endif // SPHEREOBJECT_HPP
