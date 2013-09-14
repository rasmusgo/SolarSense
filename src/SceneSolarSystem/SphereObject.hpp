#ifndef SPHEREOBJECT_HPP
#define SPHEREOBJECT_HPP

#include "../GameObject.hpp"
#include "../graphics/Model.hpp"

class SphereObject : public GameObject {
    public:
        SphereObject(Scene* parentScene, vec3f pos, vec3f scale);
        virtual ~SphereObject();

        void update(float deltaTime);
        void updateMatrix();
        void draw() const;
    private:
        Model sph;
};


#endif // SPHEREOBJECT_HPP
