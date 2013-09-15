#ifndef PLANET_HPP
#define PLANET_HPP

#include "../GameObject.hpp"
#include "../graphics/Model.hpp"

class Earth : public Planet {
    public:
        Earth(Scene* parentScene, GameObject* parentObject, vec3f scale, float orbRadius, float orbSpeed) 
        : Planet(Scene* parentScene, GameObject* parentObject), scale(scale), orbRadius(orbRadius), orbSpeed(orbSpeed);
        ~Earth();

        virtual void update(float deltaTime);
        virtual void draw() const;
        virtual void drawFrom(mat4f from) const;
        virtual void addObject(GameObject* object);


};

#endif // PLANET_HPP
