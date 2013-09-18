#ifndef EARTH_HPP
#define EARTH_HPP

#include "../GameObject.hpp"
#include "../graphics/Model.hpp"
#include "Planet.hpp"

class Earth : public Planet {
    public:
        Earth(Scene* parentScene, GameObject* parentObject, vec3f scale, float orbRadius, float orbSpeed);
        ~Earth();

        void update(float deltaTime);
        void draw() const;
        void drawFrom(mat4f from) const;

   	GameObject* parentObject;
};

#endif // EARTH_HPP
