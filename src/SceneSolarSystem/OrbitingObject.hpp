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
        void draw() const;
        void drawFrom(mat4f from) const;

    private:
        float orbSpeed;
        float orbRadius;
        Model sph;//should be probably an abstract class so this would go out
        Model orbit;
        mat4f baseMatrix;
        GameObject* parentObject;
};

#endif // ORBITINGOBJECT_HPP
