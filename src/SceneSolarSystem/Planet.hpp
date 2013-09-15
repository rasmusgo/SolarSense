#ifndef PLANET_HPP
#define PLANET_HPP

#include "../GameObject.hpp"
#include "../graphics/Model.hpp"

class Planet : public GameObject {
    public:
        Planet(Scene* parentScene, GameObject* parentObject, vec3f scale, float orbRadius, float orbSpeed) 
            : GameObject(parentScene, parentObject->pos, scale)
            , orbRadius(orbRadius), orbSpeed(orbSpeed) { }
        ~Planet(){

        }

        virtual void update(float deltaTime) = 0;
        virtual void draw() const = 0;
        //virtual void drawFrom(mat4f from) const = 0;


    private:
        float orbSpeed;
        float orbRadius;
        Model sphere;//should be probably an abstract class so this would go out
        mat4f baseMatrix;
        std::list<GameObject*> orbObjects;

};

#endif // PLANET_HPP
