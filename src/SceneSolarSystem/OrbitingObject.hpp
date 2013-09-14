#ifndef ORBITINGOBJECT_HPP
#define ORBITINGOBJECT_HPP

#include "../GameObject.hpp"
#include "../graphics/Model.hpp"

class OrbitingObject : public GameObject {
    public:
        OrbitingObject(Scene* parentScene, GameObject* parentObject, vec3f scale, float orbRadius, float orbSpeed);
        ~OrbitingObject();

        void update(float deltaTime);
        void updateMatrix();
        void draw();

        void addOrbitingObject(OrbitingObject *object);
    private:
        Model sph;//should be probably an abstract class so this would go out
};

#endif // ORBITINGOBJECT_HPP
